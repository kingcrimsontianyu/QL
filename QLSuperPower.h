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

#include "QLActor.h"
#include "GameFramework/Character.h"
#include "QLSuperPower.generated.h"

// forward declaration
class AQLCharacter;

UCLASS()
class QL_API AQLSuperPower : public AQLActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AQLSuperPower();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick( float DeltaSeconds ) override;

    virtual void ExecuteSuperPower() {};

    virtual void StopSuperPower() {};

    const FName& GetSuperPowerName() const;

    virtual void SetQLOwner(AActor* QLOwner) override;
    virtual void UnSetQLOwner() override;
    AQLCharacter* GetSuperPowerOwner();

protected:
    FName Name;
    AQLCharacter* SuperPowerOwner;
};
