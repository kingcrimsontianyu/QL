//----------------------------------------
// Quarter Life
//
// MIT license
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------

#pragma once

#include "GameFramework/Pawn.h"
#include "QLPawn.generated.h"

UCLASS()
class QL_API AQLPawn : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    AQLPawn();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick( float DeltaSeconds ) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

    virtual void SetQLOwner(AActor* QLOwner);

    virtual void UnSetQLOwner();

    AActor* GetQLOwner();

protected:
    AActor* QLOwner;
};
