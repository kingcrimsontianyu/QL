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
#include "QLSuperPower.h"
#include "QLCharacter.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLSuperPower::AQLSuperPower()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Name = "None";
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLSuperPower::BeginPlay()
{
    Super::BeginPlay();
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLSuperPower::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );
}

//------------------------------------------------------------
//------------------------------------------------------------
const FName& AQLSuperPower::GetSuperPowerName() const
{
    return Name;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLSuperPower::SetQLOwner(AActor* QLOwner)
{
    Super::SetQLOwner(QLOwner);
    this->SuperPowerOwner = Cast<AQLCharacter>(QLOwner);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLSuperPower::UnSetQLOwner()
{
    Super::SetQLOwner(nullptr);
    this->SuperPowerOwner = nullptr;
}

//------------------------------------------------------------
//------------------------------------------------------------
AQLCharacter* AQLSuperPower::GetSuperPowerOwner()
{
    return SuperPowerOwner;
}