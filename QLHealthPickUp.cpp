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
#include "QLHealthPickUp.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLHealthPickUp::AQLHealthPickUp()
{
    Increment = 10.0f;

    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
    RootComponent = BoxComponent;
    BoxComponent->InitBoxExtent(FVector(40.0f, 20.0f, 40.0f));
    BoxComponent->SetSimulatePhysics(false);
    BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

    // sound
    FireAndForgetSoundWaveList.Add("Consume", CreateFireAndForgetSoundWave(TEXT("/Game/Sounds/bite"), TEXT("HealthPickUpConsume")));

    // built-in dynamic delegate
    this->OnActorBeginOverlap.AddDynamic(this, &AQLHealthPickUp::OnOverlapBeginForActor);
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLHealthPickUp::BeginPlay()
{
    Super::BeginPlay();
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLHealthPickUp::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLHealthPickUp::OnOverlapBeginForActor(AActor* OverlappedActor, AActor* OtherActor)
{
    AQLCharacter* character = Cast<AQLCharacter>(OtherActor);
    if(character)
    {
        // increase health only when necessary
        if (character->IncrementHealth(Increment))
        {
            PlaySoundFireAndForget("Consume", character->GetActorLocation());
            this->Destroy();
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
float AQLHealthPickUp::GetHealthIncrement()
{
    return Increment;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLHealthPickUp::SetHealthIncrement(float Increment)
{
    this->Increment = Increment;
}