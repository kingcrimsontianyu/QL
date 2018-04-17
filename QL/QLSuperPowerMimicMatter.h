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
#include "QLSuperPower.h"
#include "QLDebugHelper.h"
#include "QLSuperPowerMimicMatter.generated.h"

class AQLCharacter;
class AQLSuperPowerMimicMatterPawn;

UCLASS()
class QL_API AQLSuperPowerMimicMatter : public AQLSuperPower
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AQLSuperPowerMimicMatter();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick( float DeltaSeconds ) override;

    virtual void ExecuteSuperPower() override;

    virtual void StopSuperPower() override;

    UPROPERTY()
    UTimelineComponent* CharacterTimelineComp;

    UPROPERTY()
    UTimelineComponent* MimicActorTimelineComp;

    UPROPERTY()
    UCurveFloat* TimelineCurve;
    float maxValueTimelineCurve;
    float minValueTimelineCurve;

    UPROPERTY()
    UCurveFloat* TimelineCurve2;
    float maxValueTimelineCurve2;
    float minValueTimelineCurve2;

    FOnTimelineFloat CharacterTimelineInterpFunction;
    FOnTimelineFloat MimicActorTimelineInterpFunction;

    UFUNCTION()
    void CharacterTimelineCallback(float Val);

    UFUNCTION()
    void MimicActorTimelineCallback(float Val);

    virtual void PostInitializeComponents() override;

    void DeflateCharacter();

    void InflateMimicActor();

    void PostInflateMimicActor();

protected:
    AQLSuperPowerMimicMatterPawn* MyPawn;
    AActor* MimicActor;
    FTimerHandle DelayTimerHandle;
    AController* Controller;
    FVector MimicActorScaleCache;
    FVector SuperPowerOwnerScaleCache;
    UPostProcessComponent* PostProcessComp;
    AQLDebugHelper* DebugHelper;

    UMaterial* SuperPowerMaterial;
    UMaterialInstanceDynamic* SuperPowerDynamicMaterial;
};
