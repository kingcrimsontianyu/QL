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

    SuperPowerOwner = nullptr;
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
void AQLSuperPower::SetQLOwner(AActor* QLOwner_ext)
{
    Super::SetQLOwner(QLOwner_ext);
    this->SuperPowerOwner = Cast<AQLCharacter>(QLOwner_ext);
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