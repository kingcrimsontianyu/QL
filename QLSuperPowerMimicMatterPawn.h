// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "QLSuperPowerMimicMatter.h"
#include "QLPawn.h"
#include "QLDebugHelper.h"
#include "QLSuperPowerMimicMatterPawn.generated.h"

UCLASS()
class QL_API AQLSuperPowerMimicMatterPawn : public AQLPawn
{
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    AQLSuperPowerMimicMatterPawn();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick( float DeltaSeconds ) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

    void MoveForward(float Val);

    void MoveRight(float Val);

    void Turn(float Value);

    void LookUp(float Value);

    void StopSuperPower();

    void SetMimicActor(AActor* MimicActor, UStaticMeshComponent* MimicComp);

    void UnsetMimicActor();

    void SetMovementAllowed(bool bValue);

    void SetCanStop(bool bValue);

    FVector GetCameraLocation();

protected:
    UCameraComponent* CameraComp;
    USpringArmComponent* CameraSpringArm;
    AQLDebugHelper* DebugHelper;
    AActor* MimicActor;
    UStaticMeshComponent* MimicComp;
    float ImpulseValue;
    float InstantSpeed;
    float ProjectionMaxSpeed;

    // if true, the pawn can move forward and rightward, in addition to camera movement
    // otherwise, only camera movement is permitted
    bool bMovementAllowed;

    // if true, the super power can be stopped and the player can transform into the original character form
    // this is to prevent crash caused by:
    // --- the pawn is still inflating and a timeline that uses the mimic actor is still played
    // --- the player stops the super power and destroys the mimic actor
    bool bCanStop;
};
