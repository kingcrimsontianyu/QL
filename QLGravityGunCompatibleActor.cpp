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
#include "QLGravityGunCompatibleActor.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLGravityGunCompatibleActor::AQLGravityGunCompatibleActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    RunningTimeOnHit = 0.0f;
    FixedIntervalOnHit = 0.4f;
    bTriggerOnHit = false;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    RootComponent = StaticMeshComponent;
    const ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshObj(TEXT("/Game/StarterContent/Shapes/Shape_Cube"));
    StaticMeshComponent->SetStaticMesh(StaticMeshObj.Object);
    StaticMeshComponent->SetSimulatePhysics(true);
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    StaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    StaticMeshComponent->SetLinearDamping(defaultLinearDamping);
    StaticMeshComponent->SetAngularDamping(defaultAngularDamping);
    StaticMeshComponent->SetNotifyRigidBodyCollision(true);
    StaticMeshComponent->SetWorldScale3D(FVector(1.6f));
    float zDim = StaticMeshComponent->Bounds.BoxExtent.Z; // note: extent refers to half of the side
    StaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -zDim));
    static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("/Game/StarterContent/Materials/M_Tech_Hex_Tile_Pulse"));
    if (Material.Succeeded())
    {
        this->StaticMeshComponent->SetMaterial(0, Material.Object);
    }

    // sound
    SoundComponentList.Add("Collision", CreateSoundComponent(RootComponent, TEXT("/Game/Sounds/ggca_collision"), TEXT("GGCACollision")));

    // built-in dynamic delegate
    StaticMeshComponent->OnComponentHit.AddDynamic(this, &AQLGravityGunCompatibleActor::OnComponentHitQL);
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLGravityGunCompatibleActor::BeginPlay()
{
    Super::BeginPlay();
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLGravityGunCompatibleActor::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );

    RunningTimeOnHit += DeltaTime;
    if (RunningTimeOnHit >= FixedIntervalOnHit)
    {
        RunningTimeOnHit = 0.0f;
        bTriggerOnHit = true;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLGravityGunCompatibleActor::OnComponentHitQL(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (bTriggerOnHit)
    {
        PlaySoundComponent("Collision");
    }

    bTriggerOnHit = false;
}