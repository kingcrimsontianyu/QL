//----------------------------------------
// Quarter Life
//
// MIT license
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------

#include "QL.h"
#include "QLSuperPowerTheWorld.h"
#include "QLCharacter.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLSuperPowerTheWorld::AQLSuperPowerTheWorld()
{
    Name = "TheWorld";

    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // sound
    SoundComponentList.Add("ExecuteTheWorld", CreateSoundComponent(RootComponent, TEXT("/Game/Sounds/za_warudo"), TEXT("SoundCompExecuteTheWorld")));
    SoundComponentList.Add("Expire", CreateSoundComponent(RootComponent, TEXT("/Game/Sounds/expire"), TEXT("SoundCompExpire")));

    // halo
    static ConstructorHelpers::FObjectFinder<UCurveFloat> FCurveObj(TEXT("/Game/Materials/TheWorld/C_QLTheWorldHaloCurve"));
    if (FCurveObj.Object)
    {
        FCurve = FCurveObj.Object;
    }
    HaloTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("HaloTimeline"));
    HaloTimelineInterpFunction.BindUFunction(this, FName{ TEXT("HaloCallback") });

    bIsExecutingTheWorld = false;
    FreezingPeriod = 10.0f;
    TimeDilutionFactor = 1e-4f;

    HackTotalTimer = 0.0f;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLSuperPowerTheWorld::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    HaloTimeline->AddInterpFloat(FCurve, HaloTimelineInterpFunction, FName{ TEXT("Halo") });
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLSuperPowerTheWorld::BeginPlay()
{
    Super::BeginPlay();
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLSuperPowerTheWorld::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLSuperPowerTheWorld::ExecuteSuperPower()
{
    if (bIsExecutingTheWorld == false)
    {
        // apply sound
        PlaySoundComponent("ExecuteTheWorld");

        if (SuperPowerOwner)
        {
            // stop time
            StopTime();

            // after freezing period, resume time
            // note that Timer always follows global time dilation, so to freeze time k seconds,
            // the delay parameter should be set to (k * global time dilation)
            GetWorld()->GetTimerManager().SetTimer(TheWorldTimerHandle, this, &AQLSuperPowerTheWorld::ResumeTime, 1.0f, false, FreezingPeriod * TimeDilutionFactor);
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLSuperPowerTheWorld::HaloCallback(float Val)
{
    if (SuperPowerOwner)
    {
        UMaterialInstanceDynamic* Material = SuperPowerOwner->GetSuperPowerTheWorldDynamicMaterial();
        if (Material)
        {
            Material->SetScalarParameterValue("RadiusScaleFactor", Val);
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLSuperPowerTheWorld::StopTime()
{
    // toggle flag
    bIsExecutingTheWorld = true;

    UMaterialInstanceDynamic* Material = SuperPowerOwner->GetSuperPowerTheWorldDynamicMaterial();
    if (Material)
    {
        // change global time dilution
        UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilutionFactor);

        // actors exempt from this super power
        SuperPowerOwner->CustomTimeDilation = 1.0f / TimeDilutionFactor;
        SuperPowerOwner->GetController()->CustomTimeDilation = 1.0f / TimeDilutionFactor;
        this->CustomTimeDilation = 1.0f / TimeDilutionFactor;

        // the world halo visual effect
        const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
        Material->SetScalarParameterValue("ViewPortWidth", ViewportSize.X);
        Material->SetScalarParameterValue("ViewPortHeight", ViewportSize.Y);

        // to avoid the large spike of DeltaTime, here we delay 0.1 second and then display halo
        GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &AQLSuperPowerTheWorld::PlayHaloTimeline, 1.0f, false, 0.1f * TimeDilutionFactor);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLSuperPowerTheWorld::ResumeTime()
{
    // change time dilution
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
    SuperPowerOwner->CustomTimeDilation = 1.0f;
    SuperPowerOwner->GetController()->CustomTimeDilation = 1.0f;
    this->CustomTimeDilation = 1.0f;

    // apply sound
    PlaySoundComponent("Expire");

    // toggle flag
    bIsExecutingTheWorld = false;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLSuperPowerTheWorld::PlayHaloTimeline()
{
    HaloTimeline->PlayFromStart();
}