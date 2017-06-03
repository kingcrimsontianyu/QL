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

class AQLCharacter;

// internal data for those actors who are affected by this weapon
class VictimBlob
{
public:
    TWeakObjectPtr<UPrimitiveComponent> Comp;
    TWeakObjectPtr<AActor> Actor;
};

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

    virtual void FireReleased() override;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    int GetNumOfVictim();

protected:
    void Implode();
    void Annihilate();
    void Attract();
    void DestroyMyself();

    UFUNCTION(BlueprintNativeEvent, Category = "C++Function")
    void PlayImplodeAnimation(); // xxx_Implementation() version is automatically declared

    UFUNCTION(BlueprintNativeEvent, Category = "C++Function")
    void PlayAnnihilateAnimation(); // xxx_Implementation() version is automatically declared

    UFUNCTION(BlueprintNativeEvent, Category = "C++Function")
    void ProduceGoodie(AActor* Actor); // xxx_Implementation() version is automatically declared

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    FVector2D CalculateEpicenterUV();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    float CalculateUVRadius();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    FVector CalculateGoodieSpawnLocation(const float XSpread = 50.0f, const float YSpread = 50.0f);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    FVector CalculateGoodieInitialVelocity(const float InitVelocity = 1000.0f, const float XSpread = 50.0f, const float YSpread = 50.0f, const float ZSpread = 100.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    USphereComponent* SphereComponent;

    FTimerHandle IdleTimerHandle;
    FTimerHandle ImplodeTimerHandle;
    FTimerHandle AttractTimerHandle;
    FTimerHandle AnnihilateTimerHandle;
    FTimerHandle DestroyTimerHandle;

    TArray<VictimBlob> VictimBlobList;

    UPROPERTY(BlueprintReadOnly, Category = "C++Property")
    FVector Epicenter;

    float ImpactRadius;

    TArray<FOverlapResult> OutOverlaps;

    UPROPERTY(BlueprintReadOnly, Category = "C++Property")
    APlayerController* PlayerControllerCached;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    UPostProcessComponent* PostProcessComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    UMaterial* PostProcessMaterial;

    UPROPERTY(BlueprintReadOnly, Category = "C++Property")
    UMaterialInstanceDynamic* PostProcessDynamicMaterial;
};
