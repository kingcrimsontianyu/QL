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
#include "QLPawn.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLPawn::AQLPawn()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLPawn::BeginPlay()
{
    Super::BeginPlay();
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLPawn::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );
}

//------------------------------------------------------------
// Called to bind functionality to input
//------------------------------------------------------------
void AQLPawn::SetupPlayerInputComponent(UInputComponent* InputComponent_ext)
{
    Super::SetupPlayerInputComponent(InputComponent_ext);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPawn::SetQLOwner(AActor* QLOwner_ext)
{
    this->QLOwner = QLOwner_ext;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPawn::UnSetQLOwner()
{
    this->QLOwner = nullptr;
}

//------------------------------------------------------------
//------------------------------------------------------------
AActor* AQLPawn::GetQLOwner()
{
    return QLOwner;
}