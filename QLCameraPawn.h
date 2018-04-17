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

#include "Components/SplineComponent.h"
#include "QLUtility.h"
#include "QLDebugHelper.h"
#include "GameFramework/Pawn.h"
#include "QLCameraPawn.generated.h"

// forward declaration
class AQLCameraPawn;

USTRUCT()
struct FSplineData
{
    GENERATED_USTRUCT_BODY()
    USplineComponent* SplineComponent;
    AQLCameraPawn* QLCameraPawn;
    float MoveAlongSplineTime;
    float MoveAlongSplineDistance;
    FVector PawnInitialLocation;
    FRotator PawnInitialRotation;
    FVector SplineInitialLocation;
    FRotator SplineInitialRotation;
    FVector SplineFinalLocation;
    FRotator SplineFinalRotation;
};

UCLASS()
class QL_API AQLCameraPawn : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AQLCameraPawn();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* InputComponent_ext) override;

    // Called every frame
    virtual void Tick( float DeltaSeconds ) override;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void MoveAlongSpline();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void ReverseMoveAlongSpline();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void AddSpline(USplineComponent* SplineComponent, FName SplineName);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetCurrentSpline(FName SplineName, bool bFromSplineInitialPoint = true);

    UPROPERTY(BlueprintReadOnly)
    AQLDebugHelper* DebugHelper;

    UPROPERTY()
    UTimelineComponent* MoveAlongSplineTimeline;

    UPROPERTY()
    UCurveFloat* FCurve;

    FOnTimelineFloat MoveAlongSplineTimelineInterpFunction{};

    UFUNCTION()
    void MoveAlongSplineCallback(float Val);

    virtual void PostInitializeComponents() override;

private:
    UCameraComponent* QLCameraComponent;
    TMap<FName, FSplineData> SplineList;
    FSplineData* CurrentSplineData;
};
