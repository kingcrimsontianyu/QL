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
    WeaponOwnerCached = nullptr;

    // sound
    //SoundComponentList.Add("None", CreateSoundComponent(RootComponent, TEXT("/Game/Sounds/bottle"), TEXT("SoundNoneComp")));
    //FireAndForgetSoundWaveList.Add("Start", CreateFireAndForgetSoundWave(TEXT("/Game/Sounds/za_warudo"), TEXT("SoundCompStart")));
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLWeaponRecyclerGrenade::BeginPlay()
{
    Super::BeginPlay();
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

        WeaponOwnerCached = WeaponOwner;

        WeaponOwner->DropOffWeapon(this);
        SphereComponent->SetSimulatePhysics(true);
        SphereComponent->SetCollisionProfileName(TEXT("PhysicsActor"));

        FVector Impulse = Direction * 20000.0f;

        // 3rd parameter = false means Impulse is the desired velocity to be applied and mass has no effect.
        SphereComponent->AddImpulse(Impulse, NAME_None, false);
        const float IdleTime = 2.5f;
        GetWorld()->GetTimerManager().SetTimer(IdleTimerHandle, this, &AQLWeaponRecyclerGrenade::Implode, 1.0f, false, IdleTime);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRecyclerGrenade::Implode()
{
    QLUtility::QLSay("Implode()");
    if (SphereComponent)
    {
        SphereComponent->SetSimulatePhysics(false);
        SphereComponent->SetCollisionProfileName(TEXT("NoCollision"));
    }
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
        GetActorLocation(),
        FQuat(GetActorRotation()),
        ECollisionChannel::ECC_PhysicsBody,
        FCollisionShape::MakeSphere(500.0f),
        CollisionQueryParams,
        CollisionResponseParams);
    for (int32 Index = 0; Index != OutOverlaps.Num(); ++Index)
    {
        TWeakObjectPtr<UPrimitiveComponent> Comp = OutOverlaps[Index].Component;
        // only affect actors with components that simulate physics
        if(Comp->IsSimulatingPhysics())
        {
        }
    }

    const float ImplodeTime = 1.5f;
    GetWorld()->GetTimerManager().SetTimer(ImplodeTimerHandle, this, &AQLWeaponRecyclerGrenade::Annihilate, 1.0f, false, ImplodeTime);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRecyclerGrenade::Annihilate()
{
    QLUtility::QLSay("Annihilate()");

    for (int32 Index = 0; Index != OutOverlaps.Num(); ++Index)
    {
        TWeakObjectPtr<UPrimitiveComponent> Comp = OutOverlaps[Index].Component;
        // only affect actors with components that simulate physics
        if (Comp->IsSimulatingPhysics())
        {
            TWeakObjectPtr<AActor> Actor = OutOverlaps[Index].Actor;
            ProduceChi(Actor.Get());
            Actor->Destroy();
        }
    }

    Destroy();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRecyclerGrenade::ProduceChi_Implementation(AActor* Actor)
{
}