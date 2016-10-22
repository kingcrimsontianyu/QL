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
    SoundComponentList.Add("ExecuteTheWorld", CreateSoundComponent(RootComponent, TEXT("/Game/Sounds/za_warudo"), TEXT("SoundComp")));

    // halo
    static ConstructorHelpers::FObjectFinder<UCurveFloat> FCurveObj(TEXT("/Game/Materials/TheWorld/C_QLTheWorldHaloCurve"));
    if (FCurveObj.Object)
    {
        FCurve = FCurveObj.Object;
    }
    HaloTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("HaloTimeline"));
    HaloTimelineInterpFunction.BindUFunction(this, FName{ TEXT("HaloCallback") });
    bIsExecutingTheWorld = true;
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
    // apply sound
    PlaySoundComponent("ExecuteTheWorld");

    const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

    if (SuperPowerOwner)
    {
        UMaterialInstanceDynamic* Material = SuperPowerOwner->GetSuperPowerTheWorldDynamicMaterial();
        if (Material)
        {
            Material->SetScalarParameterValue("ViewPortWidth", ViewportSize.X);
            Material->SetScalarParameterValue("ViewPortHeight", ViewportSize.Y);

            HaloTimeline->PlayFromStart();
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