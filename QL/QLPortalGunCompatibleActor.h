//----------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------

// terminology: portal gun compatible actor refers to the special
// type of wall that can have portal created on it

#pragma once

#include "QLActor.h"
#include "QLPortalGunCompatibleActor.generated.h"

UCLASS()
class QL_API AQLPortalGunCompatibleActor : public AQLActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AQLPortalGunCompatibleActor();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick( float DeltaSeconds ) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box component")
    UBoxComponent* BoxComponent;
protected:
};
