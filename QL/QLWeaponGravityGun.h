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
#include "QLGravityGunCompatibleActor.h"
#include "QLWeapon.h"
#include "QLWeaponGravityGun.generated.h"

// constant
const float attractionInstantSpeed = 1000.0f;
const float repulsionInstantSpeed = 4000.0f;
const float distanceFromCharacterToActorWhenHold = 250.0f;

UCLASS()
class QL_API AQLWeaponGravityGun : public AQLWeapon
{
    GENERATED_BODY()
public:
    AQLWeaponGravityGun();
    virtual void BeginPlay() override;
    virtual void Fire() override;
    virtual void AltFire() override;
    virtual void AltFireReleased() override;
    virtual void AltFireRepeat() override;
    virtual void Tick(float DeltaSeconds) override;
    virtual void ResetWeapon() override;
protected:
    bool bIsGravityGunCompatibleActorHeld;
    float RunningTimeAltFirePressed;
    float FixedIntervalAltFirePressed;
    float RunningTimeAltFireHeldDown;
    float FixedIntervalAltFireHeldDown;
    FRotator DeltaRotation;
    AQLGravityGunCompatibleActor* ggcActor;
};
