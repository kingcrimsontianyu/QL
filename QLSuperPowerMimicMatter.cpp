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
#include "QLCharacter.h"
#include "QLSuperPowerMimicMatter.h"
#include "QLSuperPowerMimicMatterPawn.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLSuperPowerMimicMatter::AQLSuperPowerMimicMatter()
{
    Name = "MimicMatter";

    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    MyPawn = nullptr;
    MimicActor = nullptr;

    // curve
    static ConstructorHelpers::FObjectFinder<UCurveFloat> TimelineCurveObj(TEXT("/Game/Blueprints/SuperPower/MimicMatter/Internal/C_QLMimicMatterCurve1"));
    if (TimelineCurveObj.Object)
    {
        TimelineCurve = TimelineCurveObj.Object;
        TimelineCurve->GetValueRange(minValueTimelineCurve, maxValueTimelineCurve);
    }
    static ConstructorHelpers::FObjectFinder<UCurveFloat> TimelineCurveObj2(TEXT("/Game/Blueprints/SuperPower/MimicMatter/Internal/C_QLMimicMatterCurve2"));
    if (TimelineCurveObj2.Object)
    {
        TimelineCurve2 = TimelineCurveObj2.Object;
        TimelineCurve2->GetValueRange(minValueTimelineCurve2, maxValueTimelineCurve2);
    }
    CharacterTimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("CharacterTimelineComp"));
    MimicActorTimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("MimicActorTimelineComp"));
    CharacterTimelineInterpFunction.BindUFunction(this, FName{ TEXT("CharacterTimelineCallback") });
    MimicActorTimelineInterpFunction.BindUFunction(this, FName{ TEXT("MimicActorTimelineCallback") });

    PostProcessComp = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcess"));
    PostProcessComp->bUnbound = true;
    PostProcessComp->bEnabled = false;
    PostProcessComp->SetupAttachment(RootComponent);

    // post-process
    static ConstructorHelpers::FObjectFinder<UMaterial> SuperPowerMaterialObj(TEXT("/Game/Blueprints/SuperPower/MimicMatter/Internal/M_QLMimicMatterPP"));
    if (SuperPowerMaterialObj.Succeeded())
    {
        SuperPowerMaterial = SuperPowerMaterialObj.Object;
    }
    SuperPowerDynamicMaterial = nullptr;

    // sound
    FireAndForgetSoundWaveList.Add("Transform1", CreateFireAndForgetSoundWave(TEXT("/Game/Sounds/za_warudo"), TEXT("SoundCompTransform1")));
    FireAndForgetSoundWaveList.Add("Transform2", CreateFireAndForgetSoundWave(TEXT("/Game/Sounds/za_warudo"), TEXT("SoundCompTransform2")));

    //SoundComponentList.Add("Transform1", CreateSoundComponent(RootComponent, TEXT("/Game/Sounds/za_warudo"), TEXT("SoundCompTransform1")));
    //SoundComponentList.Add("Transform2", CreateSoundComponent(RootComponent, TEXT("/Game/Sounds/za_warudo"), TEXT("SoundCompTransform2")));
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLSuperPowerMimicMatter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    CharacterTimelineComp->AddInterpFloat(TimelineCurve2, CharacterTimelineInterpFunction, FName{ TEXT("Deflate") });
    MimicActorTimelineComp->AddInterpFloat(TimelineCurve, MimicActorTimelineInterpFunction, FName{ TEXT("Inflate") });
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLSuperPowerMimicMatter::BeginPlay()
{
    Super::BeginPlay();

    SuperPowerDynamicMaterial = UMaterialInstanceDynamic::Create(SuperPowerMaterial, this);

    PostProcessComp->AddOrUpdateBlendable(SuperPowerDynamicMaterial);
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLSuperPowerMimicMatter::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLSuperPowerMimicMatter::ExecuteSuperPower()
{
    const float rayTraceRange = 10000.0f;
    FHitResult Hit = SuperPowerOwner->RayTraceFromCharacterPOV(rayTraceRange);

    if (!Hit.bBlockingHit)
    {
        QLUtility::QLSay("Mimic matter out of range.");
        return;
    }

    // if ray-tracing hits anything
    if (Hit.bBlockingHit)
    {
        //------------------------------------------------------------
        // check if the actor can be mimicked
        //------------------------------------------------------------
        AActor* ac = Hit.GetActor();

        TArray<UStaticMeshComponent*> OutComponents;
        ac->GetComponents(OutComponents);
        if (OutComponents.Num() == 0)
        {
            QLUtility::QLSay("Target has no primitive component.");
            return;
        }

        // get the first primitive component
        // for this super power to be effective, the static mesh must have collision !!!
        UStaticMeshComponent* acCompFirst = OutComponents[0];
        if (acCompFirst->Mobility != EComponentMobility::Movable ||
            !acCompFirst->IsSimulatingPhysics())
        {
            QLUtility::QLSay("Target has no movable component or is not simulating physics.");
            return;
        }

        //------------------------------------------------------------
        // to do:
        // implement a mimicable dictionary
        //------------------------------------------------------------

        //------------------------------------------------------------
        // stash current character
        //------------------------------------------------------------
        // unpossess controller
        Controller = SuperPowerOwner->GetController();
        Controller->UnPossess();

        // disable character
        SuperPowerOwnerScaleCache = SuperPowerOwner->GetActorScale3D();
        SuperPowerOwner->SetActorEnableCollision(false);
        SuperPowerOwner->SetActorTickEnabled(false);

        //------------------------------------------------------------
        // mimic the actor
        //------------------------------------------------------------
        // mimic the actor
        FVector Translation = SuperPowerOwner->GetActorLocation() - ac->GetActorLocation();
        // Transform is the additional transform applied to the new instance
        FTransform Transform(FRotator::ZeroRotator, Translation, FVector(1.0f, 1.0f, 1.0f));
        FActorSpawnParameters param;
        param.Template = ac;
        MimicActor = GetWorld()->SpawnActor<AActor>(ac->GetClass(), Transform, param);
        MimicActorScaleCache = MimicActor->GetActorScale3D();

        //------------------------------------------------------------
        // create a pawn
        //------------------------------------------------------------
        FTransform TransformPawn(SuperPowerOwner->GetActorRotation(), MimicActor->GetActorLocation(), FVector(1.0f, 1.0f, 1.0f));
        MyPawn = GetWorld()->SpawnActorDeferred<AQLSuperPowerMimicMatterPawn>(AQLSuperPowerMimicMatterPawn::StaticClass(), TransformPawn);
        MyPawn->SetQLOwner(this);

        //------------------------------------------------------------
        // attach the mimic actor to the pawn
        //------------------------------------------------------------
        OutComponents.Empty();
        MimicActor->GetComponents(OutComponents);
        // get the first primitive component
        UStaticMeshComponent* MimicCompFirst = OutComponents[0];
        MimicCompFirst->bRenderCustomDepth = true;
        // associate mimic actor with my pawn and determine the camera location
        MyPawn->SetMimicActor(MimicActor, MimicCompFirst);
        // temporarily hide mimic actor
        MimicActor->SetActorHiddenInGame(true);
        MyPawn->FinishSpawning(TransformPawn);

        //------------------------------------------------------------
        // possess pawn
        //------------------------------------------------------------
        Controller->Possess(MyPawn);
        MyPawn->SetMovementAllowed(false);

        //------------------------------------------------------------
        // animation: deflate character and inflate mimic actor
        //------------------------------------------------------------
        DeflateCharacter();
        float NewTimeLineLength = CharacterTimelineComp->GetTimelineLength() / CharacterTimelineComp->GetPlayRate();
        GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &AQLSuperPowerMimicMatter::InflateMimicActor, 1.0f, false, NewTimeLineLength);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLSuperPowerMimicMatter::StopSuperPower()
{
    if (SuperPowerOwner && MyPawn)
    {
        SuperPowerOwner->SetActorHiddenInGame(false);
        SuperPowerOwner->SetActorRelativeScale3D(FVector(1.0f));
        Controller = MyPawn->GetController();
        FVector NewLocation = MyPawn->GetActorLocation();
        FRotator NewRotation = MyPawn->GetActorRotation();
        Controller->UnPossess();
        Controller->Possess(SuperPowerOwner);

        SuperPowerOwner->SetActorHiddenInGame(false);
        SuperPowerOwner->SetActorEnableCollision(true);
        SuperPowerOwner->SetActorTickEnabled(true);
        SuperPowerOwner->SetActorLocation(NewLocation);
        SuperPowerOwner->SetActorRotation(NewRotation);

        MyPawn->UnsetMimicActor();
        MimicActor->Destroy();
        MimicActor = nullptr;
        MyPawn->Destroy();
        MyPawn = nullptr;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLSuperPowerMimicMatter::CharacterTimelineCallback(float Val)
{
    if (SuperPowerOwner)
    {
        SuperPowerOwner->SetActorScale3D(SuperPowerOwnerScaleCache * Val);

        // mask strength should go from 0 to 1
        // given the range min and max
        // mask strength = (val - min) * / (max - min)
        float maskStrength = 1.0f - (Val - minValueTimelineCurve) / (maxValueTimelineCurve - minValueTimelineCurve);
        SuperPowerDynamicMaterial->SetScalarParameterValue("MaskStrength", maskStrength);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLSuperPowerMimicMatter::MimicActorTimelineCallback(float Val)
{
    if (MimicActor)
    {
        MimicActor->SetActorScale3D(MimicActorScaleCache * Val);
        float maskStrength = 1.0f - (Val - minValueTimelineCurve2) / (maxValueTimelineCurve2 - minValueTimelineCurve2);
        SuperPowerDynamicMaterial->SetScalarParameterValue("MaskStrength", maskStrength);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLSuperPowerMimicMatter::DeflateCharacter()
{
    PostProcessComp->bEnabled = true;

    const float PlayRate = 1.0f;
    CharacterTimelineComp->SetPlayRate(PlayRate);
    // non-blocking
    CharacterTimelineComp->PlayFromStart();

    // apply sound
    PlaySoundFireAndForget("Transform1", SuperPowerOwner->GetActorLocation());
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLSuperPowerMimicMatter::InflateMimicActor()
{
    SuperPowerOwner->SetActorHiddenInGame(true);
    MimicActor->SetActorHiddenInGame(false);

    // non-blocking
    MimicActorTimelineComp->PlayFromStart();

    GetWorld()->GetTimerManager().ClearTimer(DelayTimerHandle);
    float NewTimeLineLength = MimicActorTimelineComp->GetTimelineLength() / MimicActorTimelineComp->GetPlayRate();
    GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &AQLSuperPowerMimicMatter::PostInflateMimicActor, 1.0f, false, NewTimeLineLength);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLSuperPowerMimicMatter::PostInflateMimicActor()
{
    MyPawn->SetMovementAllowed(true);
    MyPawn->SetCanStop(true);
    PostProcessComp->bEnabled = false;
}