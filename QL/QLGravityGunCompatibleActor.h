//----------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------

// terminology: gravity gun compatible actor refers to the object
// that can be attracted and repulsed by the gravity gun

#pragma once

#include "QLActor.h"
#include "QLGravityGunCompatibleActor.generated.h"

// constant
const float defaultLinearDamping = 1.0f;
const float defaultAngularDamping = 0.5f;
const float defaultBoxComponentDim = 80.0f;

UCLASS()
class QL_API AQLGravityGunCompatibleActor : public AQLActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AQLGravityGunCompatibleActor();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick( float DeltaSeconds ) override;

    UFUNCTION()
    void OnComponentHitQL(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
    float RunningTimeOnHit;
    float FixedIntervalOnHit;
    bool bTriggerOnHit;
};
