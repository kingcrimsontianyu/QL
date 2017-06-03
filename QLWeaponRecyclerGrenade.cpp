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
#include "QLWeaponRecyclerGrenade.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLWeaponRecyclerGrenade::AQLWeaponRecyclerGrenade()
{
    PrimaryActorTick.bCanEverTick = true;
    SphereComponent = nullptr;
    Name = "RecyclerGrenade";

    ImpactRadius = 500.0f;
    PostProcessComp = nullptr;
    PostProcessMaterial = nullptr;
    PostProcessDynamicMaterial = nullptr;

    // sound
    //SoundComponentList.Add("None", CreateSoundComponent(RootComponent, TEXT("/Game/Sounds/bottle"), TEXT("SoundNoneComp")));
    //FireAndForgetSoundWaveList.Add("Start", CreateFireAndForgetSoundWave(TEXT("/Game/Sounds/za_warudo"), TEXT("SoundCompStart")));
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLWeaponRecyclerGrenade::BeginPlay()
{
    // BP BeginPlay() is actually called in Super::BeginPlay()
    Super::BeginPlay();

    // so these variables have been safely set
    if (PostProcessComp && PostProcessMaterial)
    {
        PostProcessDynamicMaterial = UMaterialInstanceDynamic::Create(PostProcessMaterial, this);
        PostProcessComp->AddOrUpdateBlendable(PostProcessDynamicMaterial);
        PostProcessComp->bUnbound = true;
        PostProcessComp->bEnabled = false;
    }
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLWeaponRecyclerGrenade::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRecyclerGrenade::FireReleased()
{
    if (SphereComponent)
    {
        const float ThrowRange = 10000.0f;
        UCameraComponent* CameraComp = WeaponOwner->GetQLCameraComponent();
        FVector Direction = CameraComp->GetComponentLocation() - GetActorLocation() + ThrowRange * CameraComp->GetForwardVector();
        Direction.Normalize();

        PlayerControllerCached = Cast<APlayerController>(WeaponOwner->GetController());

        WeaponOwner->DropOffWeapon(this);
        SphereComponent->SetSimulatePhysics(true);
        SphereComponent->SetCollisionProfileName(TEXT("PhysicsActor"));

        FVector Impulse = Direction * 20000.0f;

        // 3rd parameter = false means Impulse is the desired velocity to be applied and mass has no effect.
        SphereComponent->AddImpulse(Impulse, NAME_None, false);
        const float IdleTime = 2.5f;
        // delay by IdleTime, then Implode()
        GetWorld()->GetTimerManager().SetTimer(IdleTimerHandle, this, &AQLWeaponRecyclerGrenade::Implode, 1.0f, false, IdleTime);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRecyclerGrenade::Implode()
{
    if (SphereComponent)
    {
        SphereComponent->SetSimulatePhysics(false);
        SphereComponent->SetCollisionProfileName(TEXT("NoCollision"));
    }
    Epicenter = GetActorLocation();
    SetActorHiddenInGame(true);

    // reference about the concepts: https://docs.unrealengine.com/latest/INT/Engine/Physics/Collision/Reference/index.html
    // collapse space and attract objects
    // TArray < struct FOverlapResult > & OutOverlaps,
    // const FVector & Pos,
    // const FQuat & Rot,
    // ECollisionChannel TraceChannel,
    // const FCollisionShape & CollisionShape,
    // const FCollisionQueryParams & Params,
    // const FCollisionResponseParams & ResponseParam
    FCollisionQueryParams CollisionQueryParams;
    FCollisionResponseParams CollisionResponseParams;
    GetWorld()->OverlapMultiByChannel(OutOverlaps,
        Epicenter,
        FQuat(GetActorRotation()),
        ECollisionChannel::ECC_PhysicsBody,
        FCollisionShape::MakeSphere(ImpactRadius),
        CollisionQueryParams,
        CollisionResponseParams);
    for (int32 Index = 0; Index != OutOverlaps.Num(); ++Index)
    {
        TWeakObjectPtr<UPrimitiveComponent> Comp = OutOverlaps[Index].Component;
        // only affect actors with components that simulate physics
        if(Comp->IsSimulatingPhysics())
        {
            TWeakObjectPtr<AActor> Actor = OutOverlaps[Index].Actor;
            VictimBlobList.Add(VictimBlob());
            VictimBlobList.Last().Comp = Comp;
            VictimBlobList.Last().Actor = Actor;
        }
    }

    // visual
    PostProcessComp->bEnabled = true;
    PlayImplodeAnimation();

    // ApplyImpulseToComp repeatedly for every InRate seconds
    GetWorld()->GetTimerManager().SetTimer(AttractTimerHandle, this, &AQLWeaponRecyclerGrenade::Attract, 0.01f, true, 0.0f);

    // delay by ImplodeTime, then Annihilate()
    const float ImplodeTime = 1.5f;
    GetWorld()->GetTimerManager().SetTimer(ImplodeTimerHandle, this, &AQLWeaponRecyclerGrenade::Annihilate, 1.0f, false, ImplodeTime);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRecyclerGrenade::Attract()
{
    //QLUtility::QLSay("Attract()");
    for (int32 Index = 0; Index != VictimBlobList.Num(); ++Index)
    {
        FVector Direction = Epicenter - VictimBlobList[Index].Comp->GetComponentLocation();
        Direction.Normalize();
        FVector Impulse = Direction * VictimBlobList[Index].Comp->GetMass() * 0.2f;
        VictimBlobList[Index].Comp->AddImpulse(Impulse, NAME_None, true);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRecyclerGrenade::Annihilate()
{
    GetWorld()->GetTimerManager().ClearTimer(AttractTimerHandle);

    PlayAnnihilateAnimation();

    for (int32 Index = 0; Index != VictimBlobList.Num(); ++Index)
    {
        ProduceGoodie(VictimBlobList[Index].Actor.Get());
        VictimBlobList[Index].Actor->Destroy();
    }

    // todo: currently Destroy() is implemented in BP using a dirty timerhandle-based method
    // because BP does not naturally support delay in for-loop
    // need to look into the way to implement that in c++
    // delay by AnnihilateTime, then Destroy()
    //const float AnnihilateTime = 1.5f;
    //GetWorld()->GetTimerManager().SetTimer(AnnihilateTimerHandle, this, &AQLWeaponRecyclerGrenade::DestroyMyself, 1.0f, false, AnnihilateTime);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRecyclerGrenade::PlayImplodeAnimation_Implementation()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRecyclerGrenade::PlayAnnihilateAnimation_Implementation()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRecyclerGrenade::ProduceGoodie_Implementation(AActor* Actor)
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRecyclerGrenade::DestroyMyself()
{
    Destroy();
}

//------------------------------------------------------------
//------------------------------------------------------------
FVector2D AQLWeaponRecyclerGrenade::CalculateEpicenterUV()
{
    FVector2D UV;
    if (PlayerControllerCached)
    {
        FVector2D ScreenLocation;
        PlayerControllerCached->ProjectWorldLocationToScreen(Epicenter, ScreenLocation);
        int X, Y;
        PlayerControllerCached->GetViewportSize(X, Y);
        UV.X = ScreenLocation.X / static_cast<float>(X);
        UV.Y = ScreenLocation.Y / static_cast<float>(Y);
    }

    return UV;
}

//------------------------------------------------------------
//------------------------------------------------------------
float AQLWeaponRecyclerGrenade::CalculateUVRadius()
{
    float Radius = 0.0f;

    if (PlayerControllerCached)
    {
        FVector2D ScreenLocation;
        PlayerControllerCached->ProjectWorldLocationToScreen(Epicenter, ScreenLocation);

        FVector AnotherWorldLocation = Epicenter + FVector(0.0f, 0.0f, ImpactRadius);
        FVector2D AnotherScreenLocation;
        PlayerControllerCached->ProjectWorldLocationToScreen(AnotherWorldLocation, AnotherScreenLocation);

        FVector2D Diff = ScreenLocation - AnotherScreenLocation;

        // normalize to uv
        int X, Y;
        PlayerControllerCached->GetViewportSize(X, Y);
        Diff.X /= static_cast<float>(X);
        Diff.Y /= static_cast<float>(Y);

        Radius = Diff.Size();
    }

    return Radius;
}

//------------------------------------------------------------
//------------------------------------------------------------
FVector AQLWeaponRecyclerGrenade::CalculateGoodieSpawnLocation(const float XSpread, const float YSpread)
{
    FVector Location;
    Location.X = Epicenter.X + FMath::FRandRange(-1.0f, 1.0f) * XSpread;
    Location.Y = Epicenter.Y + FMath::FRandRange(-1.0f, 1.0f) * YSpread;
    Location.Z = Epicenter.Z;
    return Location;
}

//------------------------------------------------------------
//------------------------------------------------------------
FVector AQLWeaponRecyclerGrenade::CalculateGoodieInitialVelocity(const float InitVelocity, const float XSpread, const float YSpread, const float ZSpread)
{
    FVector Velocity(0.0f, 0.0f, InitVelocity);
    Velocity.X += FMath::FRandRange(-1.0f, 1.0f) * XSpread;
    Velocity.Y += FMath::FRandRange(-1.0f, 1.0f) * YSpread;
    Velocity.Z += FMath::FRandRange(-1.0f, 1.0f) * ZSpread;
    return Velocity;
}

//------------------------------------------------------------
//------------------------------------------------------------
int AQLWeaponRecyclerGrenade::GetNumOfVictim()
{
    return VictimBlobList.Num();
}