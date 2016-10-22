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
#include "QLSuperPower.h"
#include "QLSuperPowerTheWorld.generated.h"

UCLASS()
class QL_API AQLSuperPowerTheWorld : public AQLSuperPower
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AQLSuperPowerTheWorld();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick( float DeltaSeconds ) override;

    virtual void PostInitializeComponents() override;

    virtual void ExecuteSuperPower() override;

    UPROPERTY()
    UTimelineComponent* HaloTimeline;

    UPROPERTY()
    UCurveFloat* FCurve;

    FOnTimelineFloat HaloTimelineInterpFunction{};

    UFUNCTION()
    void HaloCallback(float Val);

    void StopTime();
    void ResumeTime();

protected:
    bool bIsExecutingTheWorld;
    float FreezingPeriod;
    float TimeDilutionFactor;
    FTimerHandle TimerHandle;
};
