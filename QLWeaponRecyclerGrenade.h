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
#include "QLWeapon.h"
#include "QLWeaponRecyclerGrenade.generated.h"

UCLASS()
class QL_API AQLWeaponRecyclerGrenade : public AQLWeapon
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AQLWeaponRecyclerGrenade();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick( float DeltaSeconds ) override;

    virtual void Fire() override;

protected:
    int NumRecyclerGrenade;
};
