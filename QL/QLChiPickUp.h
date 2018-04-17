//----------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------
#pragma once

#include "QLActor.h"
#include "QLCharacter.h"
#include "QLChiPickUp.generated.h"

UCLASS()
class QL_API AQLChiPickUp : public AQLActor
{
    GENERATED_BODY()

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Sets default values for this actor's properties
    AQLChiPickUp();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void OnOverlapBeginForActor(AActor* OverlappedActor, AActor* OtherActor);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    float GetChiIncrement();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetChiIncrement(float Increment_ext);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    float Increment;
};
