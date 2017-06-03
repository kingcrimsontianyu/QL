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