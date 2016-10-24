//----------------------------------------
// Quarter Life
//
// MIT license
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------

//----------------------------------------
// Modifying actor's custom time dilation can curiously cause
// DeltaTime of Tick() to be abnormally large, greater than 8
// seconds, although in reality no appreciable lag is observed.
// This abnormal DeltaTime makes the original implementation unable
// to work correctly (halo not displayed at all).
// The solution is extraordinarily simple: just delay by 0.1 seconds.
//
// Note that Tick() and DeltaTime are supposed to be independent of
// time dilation at all.
//
// In order for the super power to take effect,
// on each level BP:
// --- there needs to be a post process volume
// --- the dynamic material instance based on M_QLTheWorldHaloPP
//     should be assigned to that post process volume
// --- this assignment is done via EventBeginPlay() of the level
// --- the dynamic material instance is a data member of the character
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

    void PlayHaloTimeline();

protected:
    bool bIsExecutingTheWorld;
    float FreezingPeriod;
    float TimeDilationFactor;
    FTimerHandle TheWorldTimerHandle;
    FTimerHandle DelayTimerHandle;
    float HackTotalTimer;
};
