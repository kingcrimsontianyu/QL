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
#include "QLCharacter.h"
#include "QLHealthPickUp.generated.h"

UCLASS()
class QL_API AQLHealthPickUp : public AQLActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AQLHealthPickUp();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick( float DeltaSeconds ) override;

    UFUNCTION()
    void OnOverlapBeginForActor(AActor* OverlappedActor, AActor* OtherActor);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    float GetHealthIncrement();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetHealthIncrement(float Increment);

protected:
    float Increment;
};
