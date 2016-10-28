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
#include "QLCameraPawn.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLCameraPawn::AQLCameraPawn()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    QLCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    QLCameraComponent->SetupAttachment(RootComponent);
    QLCameraComponent->bUsePawnControlRotation = true;

    // spline
    CurrentSplineData = nullptr;
    static ConstructorHelpers::FObjectFinder<UCurveFloat> FCurveObj(TEXT("/Game/Blueprints/PlayerUMG/C_QLMoveAlongSplineCurve"));
    if (FCurveObj.Object)
    {
        FCurve = FCurveObj.Object;
    }
    MoveAlongSplineTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MoveAlongSplineTimeline"));
    MoveAlongSplineTimelineInterpFunction.BindUFunction(this, FName{ TEXT("MoveAlongSplineCallback") });
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCameraPawn::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    MoveAlongSplineTimeline->AddInterpFloat(FCurve, MoveAlongSplineTimelineInterpFunction, FName{ TEXT("MoveAlongSpline") });
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCameraPawn::BeginPlay()
{
    Super::BeginPlay();

    //DebugHelper = GetWorld()->SpawnActor<AQLDebugHelper>(AQLDebugHelper::StaticClass());
    //DebugHelper->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
    //DebugHelper->SetActorRelativeLocation(FVector(200.0f, 0.0f, 0.0f));
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCameraPawn::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCameraPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
    Super::SetupPlayerInputComponent(InputComponent);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCameraPawn::MoveAlongSpline()
{
    if (CurrentSplineData)
    {
        MoveAlongSplineTimeline->Play();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCameraPawn::ReverseMoveAlongSpline()
{
    if (CurrentSplineData)
    {
        MoveAlongSplineTimeline->Reverse();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCameraPawn::AddSpline(USplineComponent* SplineComponent, FName SplineName)
{
    FSplineData sd;
    sd.SplineComponent = SplineComponent;
    sd.QLCameraPawn = this;
    sd.MoveAlongSplineTime = 2.0f;
    sd.MoveAlongSplineDistance = SplineComponent->GetSplineLength();
    sd.PawnInitialLocation = sd.QLCameraPawn->GetActorLocation();
    sd.PawnInitialRotation = sd.QLCameraPawn->GetActorRotation();
    sd.SplineInitialLocation = SplineComponent->GetLocationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World);
    sd.SplineInitialRotation = SplineComponent->GetRotationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World);
    sd.SplineFinalLocation = SplineComponent->GetLocationAtDistanceAlongSpline(sd.MoveAlongSplineDistance, ESplineCoordinateSpace::World);
    sd.SplineFinalRotation = SplineComponent->GetRotationAtDistanceAlongSpline(sd.MoveAlongSplineDistance, ESplineCoordinateSpace::World);

    SplineList.Add(SplineName, sd);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCameraPawn::SetCurrentSpline(FName SplineName, bool bFromSplineInitialPoint)
{
    FSplineData* sd = SplineList.Find(SplineName);
    if (sd)
    {
        CurrentSplineData = sd;

        if (bFromSplineInitialPoint)
        {
            SetActorLocation(CurrentSplineData->SplineInitialLocation);
            GetController()->SetControlRotation(CurrentSplineData->PawnInitialRotation + CurrentSplineData->SplineInitialRotation);
        }
        else
        {
            SetActorLocation(CurrentSplineData->SplineFinalLocation);
            GetController()->SetControlRotation(CurrentSplineData->PawnInitialRotation + CurrentSplineData->SplineFinalRotation);
        }

        MoveAlongSplineTimeline->SetPlayRate(1.0f / CurrentSplineData->MoveAlongSplineTime);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLCameraPawn::MoveAlongSplineCallback(float Val)
{
    if (CurrentSplineData)
    {
        float CurrentDistance = Val * CurrentSplineData->MoveAlongSplineDistance;
        FVector Location = CurrentSplineData->SplineComponent->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
        SetActorLocation(Location);

        FRotator Rotation = CurrentSplineData->SplineComponent->GetRotationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
        Rotation += CurrentSplineData->PawnInitialRotation;
        GetController()->SetControlRotation(Rotation);
    }
}