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
#include "QLHealthPickUp.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLHealthPickUp::AQLHealthPickUp()
{
    Increment = 0.0f;

    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

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
            PlaySoundFireAndForget("Consume");
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
void AQLHealthPickUp::SetHealthIncrement(float Increment_ext)
{
    Increment = Increment_ext;
}