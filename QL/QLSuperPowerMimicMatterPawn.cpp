//----------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------

#include "QL.h"
#include "QLSuperPowerMimicMatterPawn.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLSuperPowerMimicMatterPawn::AQLSuperPowerMimicMatterPawn()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    QLOwner = nullptr;
    MimicActor = nullptr;
    MimicComp = nullptr;
    InstantSpeed = 50.0f;
    ProjectionMaxSpeed = 500.0f;
    bMovementAllowed = true;
    bCanStop = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

    CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
    CameraSpringArm->SetupAttachment(RootComponent);
    CameraSpringArm->TargetArmLength = 200.f;
    CameraSpringArm->bEnableCameraLag = true;
    CameraSpringArm->CameraLagSpeed = 3.0f;
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("PawnCamera"));
    CameraComp->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLSuperPowerMimicMatterPawn::BeginPlay()
{
    Super::BeginPlay();

    // debug
    //DebugHelper = GetWorld()->SpawnActor<AMimicDebugHelper>(AMimicDebugHelper::StaticClass());
    //DebugHelper->AttachToComponent(CameraSpringArm, FAttachmentTransformRules::SnapToTargetIncludingScale);
    //DebugHelper->SetActorRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    //DebugHelper->SetActorScale3D(FVector(4.0f))
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLSuperPowerMimicMatterPawn::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );

    if (MimicActor)
    {
        RootComponent->SetWorldLocation(MimicActor->GetActorLocation());
    }
}

//------------------------------------------------------------
// Called to bind functionality to input
//------------------------------------------------------------
void AQLSuperPowerMimicMatterPawn::SetupPlayerInputComponent(UInputComponent* InputComponent_ext)
{
    Super::SetupPlayerInputComponent(InputComponent_ext);

    InputComponent->BindAxis("MoveForward", this, &AQLSuperPowerMimicMatterPawn::MoveForward);
    InputComponent->BindAxis("MoveRight", this, &AQLSuperPowerMimicMatterPawn::MoveRight);
    InputComponent->BindAxis("Turn", this, &AQLSuperPowerMimicMatterPawn::Turn);
    InputComponent->BindAxis("LookUp", this, &AQLSuperPowerMimicMatterPawn::LookUp);
    InputComponent->BindAction("ExecuteSuperPower", IE_Released, this, &AQLSuperPowerMimicMatterPawn::StopSuperPower);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLSuperPowerMimicMatterPawn::MoveForward(float Value)
{
    if (Value != 0.0f && MimicComp && bMovementAllowed)
    {
        float Projection = FVector::DotProduct(MimicComp->GetComponentVelocity(), GetActorForwardVector());
        if (FGenericPlatformMath::Abs(Projection) < ProjectionMaxSpeed // projection has not exceeded the limit
            || (Projection < 0.0f && Value > 0.0f) // wanted speed has opposite direction to current speed
            || (Projection > 0.0f && Value < 0.0f)) // wanted speed has opposite direction to current speed
        {
            MimicComp->AddImpulse(GetActorForwardVector() * ImpulseValue * Value);
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLSuperPowerMimicMatterPawn::MoveRight(float Value)
{
    if (Value != 0.0f && MimicComp && bMovementAllowed)
    {
        float Projection = FVector::DotProduct(MimicComp->GetComponentVelocity(), GetActorRightVector());
        if (FGenericPlatformMath::Abs(Projection) < ProjectionMaxSpeed // projection has not exceeded the limit
            || (Projection < 0.0f && Value > 0.0f) // wanted speed has opposite direction to current speed
            || (Projection > 0.0f && Value < 0.0f)) // wanted speed has opposite direction to current speed
        {
            MimicComp->AddImpulse(GetActorRightVector() * ImpulseValue * Value);
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLSuperPowerMimicMatterPawn::Turn(float Value)
{
    if (Value != 0.0f)
    {
        FRotator NewRotation = GetActorRotation();
        NewRotation.Yaw += Value;
        SetActorRotation(NewRotation);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLSuperPowerMimicMatterPawn::LookUp(float Value)
{
    if (Value != 0.0f)
    {
        FRotator NewRotation = CameraSpringArm->GetComponentRotation();
        NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch - Value, -80.0f, 30.0f);
        CameraSpringArm->SetWorldRotation(NewRotation);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLSuperPowerMimicMatterPawn::StopSuperPower()
{
    if (QLOwner && bCanStop)
    {
        AQLSuperPowerMimicMatter* QLOwnerCast = Cast<AQLSuperPowerMimicMatter>(QLOwner);
        if (QLOwnerCast)
        {
            QLOwnerCast->StopSuperPower();
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLSuperPowerMimicMatterPawn::SetMimicActor(AActor* MimicActor_ext, UStaticMeshComponent* MimicComp_ext)
{
    this->MimicActor = MimicActor_ext;
    this->MimicComp = MimicComp_ext;

    // calculate impulse to be applied to the component
    ImpulseValue = MimicComp->GetMass() * InstantSpeed;

    // set up camera
    FVector CameraSpringArmLocation = { 0.0f, 0.0f, 0.0f };
    FBox Bound = MimicActor->GetComponentsBoundingBox();
    FVector Extent = Bound.GetExtent();
    CameraSpringArmLocation.X = -Extent.X * 2.0f;
    CameraSpringArmLocation.Z = Extent.Z * 1.5f;
    FVector Translation = Bound.GetCenter() - GetActorLocation();
    CameraSpringArmLocation += Translation;
    CameraSpringArm->SetRelativeLocation(CameraSpringArmLocation);
    // adjust pitch value
    CameraSpringArm->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLSuperPowerMimicMatterPawn::UnsetMimicActor()
{
    MimicActor = nullptr;
    MimicComp = nullptr;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLSuperPowerMimicMatterPawn::SetMovementAllowed(bool bValue)
{
    bMovementAllowed = bValue;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLSuperPowerMimicMatterPawn::SetCanStop(bool bValue)
{
    bCanStop = bValue;
}

//------------------------------------------------------------
//------------------------------------------------------------
FVector AQLSuperPowerMimicMatterPawn::GetCameraLocation()
{
    return CameraComp->GetComponentLocation();
}