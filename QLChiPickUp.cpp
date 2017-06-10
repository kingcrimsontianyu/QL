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
#include "QLChiPickUp.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLChiPickUp::AQLChiPickUp()
{
    Increment = 0.0f;

    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // built-in dynamic delegate
    this->OnActorBeginOverlap.AddDynamic(this, &AQLChiPickUp::OnOverlapBeginForActor);
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLChiPickUp::BeginPlay()
{
    Super::BeginPlay();
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLChiPickUp::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLChiPickUp::OnOverlapBeginForActor(AActor* OverlappedActor, AActor* OtherActor)
{
    AQLCharacter* character = Cast<AQLCharacter>(OtherActor);
    if (character)
    {
        // increase health only when necessary
        if (character->IncrementChi(Increment))
        {
            PlaySoundFireAndForget("Consume");
            this->Destroy();
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
float AQLChiPickUp::GetChiIncrement()
{
    return Increment;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLChiPickUp::SetChiIncrement(float Increment_ext)
{
    Increment = Increment_ext;
}