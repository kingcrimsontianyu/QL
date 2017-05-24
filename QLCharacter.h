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

#include "QLUtility.h"
#include "QLDebugHelper.h"
#include "QLWeaponGravityGun.h"
#include "QLWeaponPortalGun.h"
#include "QLSuperPowerTheWorld.h"
#include "QLSuperPowerMimicMatter.h"
#include "GameFramework/Character.h"
#include "QLCharacter.generated.h"

// constant
const int maxNumWeapon = 10;
const float nextToPlayerThreshold = 400.0f;

UCLASS()
class QL_API AQLCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    bool IsWeaponEquipped(const FName& Name);
    void ChangeCurrentWeapon(AQLWeapon* Weapon);

    bool IsSuperPowerEquipped(const FName& Name);

    bool bCanChangeCurrentWeapon;
    void ChangeCurrentSuperPower(AQLSuperPower* Weapon);

    void AddToInventory(AQLActor* QLActor);
    void RemoveFromInventory(AQLActor* QLActor);
    void ShowInventory();
    void Zoom();

    AQLCharacter();
    ~AQLCharacter();
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick( float DeltaSeconds ) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

    virtual void Landed(const FHitResult& Hit) override;
    virtual void Falling() override;

    FHitResult RayTraceFromCharacterPOV();

    FHitResult RayTraceFromCharacterPOV(float rayTraceRange);

    bool IsObjectNextToCharacter(AQLGravityGunCompatibleActor* ggcActor);

    // implementation makes sure this function can only be called once
    void UnlockAllWeaponAndSuperPower();

    // Handles input for moving forward and backward.
    UFUNCTION()
    void MoveForward(float Value);

    UFUNCTION()
    void MoveRight(float Value);

    // Sets jump flag when key is pressed.
    UFUNCTION()
    void StartJump();

    // Clears jump flag when key is released.
    UFUNCTION()
    void StopJump();

    // Clears jump flag when key is released.
    UFUNCTION()
    void ToggleCrouch();

    UFUNCTION()
    void ToggleSprint();

    UFUNCTION()
    bool CanSprint() const;

    UFUNCTION()
    void Sprint();

    UFUNCTION()
    void UnSprint();

    UFUNCTION()
    void Fire();

    UFUNCTION()
    void AltFire();

    UFUNCTION()
    void AltFireReleased();

    UFUNCTION()
    void SwitchToGravityGun();

    UFUNCTION()
    void SwitchToPortalGun();

    UFUNCTION()
    void SwitchToNeutronAWP();

    UFUNCTION()
    void SwitchToLastWeapon();

    UFUNCTION()
    void ExecuteSuperPower();

    void SwitchToWeapon(const FName& Name);

    void PickUpWeapon(AQLWeapon* Weapon);
    AQLWeapon* GetCurrentWeapon() const;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void PickUpSuperPower(AQLSuperPower* SuperPower);

    AQLSuperPower* GetCurrentSuperPower() const;

    UAudioComponent* CreateSoundComponent(USceneComponent*& RootComponent, const TCHAR* soundPath, const TCHAR* soundName);
    USoundWave* CreateFireAndForgetSoundWave(const TCHAR* SoundPath, const TCHAR* SoundName);
    void PlaySoundComponent(const FName& SoundName);
    void PlaySoundFireAndForget(const FName& SoundName, const FVector& Location);

    void Test();
    UPhysicsHandleComponent* PhysicsHandle;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    UMaterialInstanceDynamic* GetSuperPowerTheWorldDynamicMaterial();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    float GetHealth() const;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    float GetChi() const;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    float GetVelocityLength() const;

    UFUNCTION(BlueprintNativeEvent, Category = "C++Function")
    bool IncrementHealth(float increment); // xxx_Implementation() version is automatically declared

    UFUNCTION(BlueprintNativeEvent, Category = "C++Function")
    bool IncrementChi(float increment); // xxx_Implementation() version is automatically declared

protected:
    bool bIsFalling;
    bool bCanDoubleJump;

    // bool bHaveSprintAbility; // whether player has gained the ability
    bool bIsSprinting; // whether player is sprinting
    bool bWantToSprint; // player wants to spring
    float MaxWalkSpeed;
    float MaxWalkSpeedCrouched;
    float Health;
    float MaxHealth;
    float Chi;
    float MaxChi;
    bool bAllWeaponAndSuperPowerUnlockable;
    AQLWeapon* CurrentWeapon;
    AQLWeapon* LastWeapon;
    TMap<FName, AQLWeapon*> WeaponList;
    AQLSuperPower* CurrentSuperPower;
    TMap<FName, AQLSuperPower*> SuperPowerList;
    TMap<FString, AActor*> Inventory;
    USoundAttenuation* SoundNoAttenuation;
    USoundAttenuation* SoundAttenuation;
    TMap<FName, UAudioComponent*> SoundComponentList;
    TMap<FName, USoundWave*> FireAndForgetSoundWaveList;
    UCameraComponent* QLCameraComponent;
    AQLDebugHelper* DebugHelper;

    UMaterial* SuperPowerTheWorldMaterial;
    UMaterialInstanceDynamic* SuperPowerTheWorldDynamicMaterial;
};
