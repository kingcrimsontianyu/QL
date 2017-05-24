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
void AQLPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
    Super::SetupPlayerInputComponent(InputComponent);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPawn::SetQLOwner(AActor* QLOwner)
{
    this->QLOwner = QLOwner;
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