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

#include "QLGameInstance.h"
#include "QLDebugHelper.h"
#include "QLWeaponGravityGun.h"
#include "QLWeaponPortalGun.h"
#include "QLWeaponRecyclerGrenade.h"
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
    virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent_ext) override;

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
    void FireReleased();

    UFUNCTION()
    void AltFire();

    UFUNCTION()
    void AltFireReleased();

    // todo: refactor
    UFUNCTION()
    void SwitchToGravityGun();

    // todo: refactor
    UFUNCTION()
    void SwitchToPortalGun();

    // todo: refactor
    UFUNCTION()
    void SwitchToRecyclerGrenade();

    UFUNCTION()
    void SwitchToLastWeapon();

    UFUNCTION()
    void ExecuteSuperPower();

    void SwitchToWeapon(const FName& Name);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void PickUpWeapon(AQLWeapon* Weapon);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void DropOffWeapon(AQLWeapon* Weapon);

    AQLWeapon* GetCurrentWeapon() const;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void PickUpSuperPower(AQLSuperPower* SuperPower);

    AQLSuperPower* GetCurrentSuperPower() const;

    void PlaySoundComponent(const FName& SoundName);

    void PlaySoundFireAndForget(const FName& SoundName);

    void Test();
    UPhysicsHandleComponent* PhysicsHandle;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    UMaterialInstanceDynamic* GetSuperPowerTheWorldDynamicMaterial();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    float GetHealth() const;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    float GetHealthPercent() const;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    float GetChi() const;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    float GetChiPercent() const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++Function")
    bool IncrementHealth(float increment); // xxx_Implementation() version is automatically declared

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++Function")
    bool IncrementChi(float increment); // xxx_Implementation() version is automatically declared

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    USkeletalMeshComponent* GetArm() const;

    UCameraComponent* GetQLCameraComponent() const;

    void ReplayCurrentLevel();

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
    float HealthPercent;

    float Chi;
    float MaxChi;
    float ChiPercent;
    
    AQLWeapon* CurrentWeapon;
    AQLWeapon* LastWeapon;
    TMap<FName, AQLWeapon*> WeaponList;
    AQLSuperPower* CurrentSuperPower;
    TMap<FName, AQLSuperPower*> SuperPowerList;
    TMap<FString, AActor*> Inventory;
    USoundAttenuation* SoundNoAttenuation;
    USoundAttenuation* SoundAttenuation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    TMap<FName, TAssetPtr<USoundWave>> SoundComponentAssetList;

    TMap<FName, UAudioComponent*> SoundComponentList;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    TMap<FName, TAssetPtr<USoundWave>> FireAndForgetSoundAssetList;

    TMap<FName, USoundWave*> FireAndForgetSoundList;

    UAudioComponent* CreateSoundComponent(FName SoundName, TAssetPtr<USoundWave> SoundWaveAsset);

    USoundWave* CreateFireAndForgetSound(FName SoundName, TAssetPtr<USoundWave> SoundWaveAsset);

    UCameraComponent* QLCameraComponent;
    AQLDebugHelper* DebugHelper;

    UMaterial* SuperPowerTheWorldMaterial;
    UMaterialInstanceDynamic* SuperPowerTheWorldDynamicMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    USkeletalMeshComponent* ArmSkeletalMeshComponent; // initialized to nullptr in c++, to be set in BP

    bool bAllWeaponAndSuperPowerUnlockable;
};
