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
#include "QLGameInstance.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "QLActor.generated.h"

UCLASS()
class QL_API AQLActor : public AActor
{
    GENERATED_BODY()

public:
    AQLActor();
    virtual void BeginPlay() override;
    virtual void Tick( float DeltaSeconds ) override;
    virtual void SetQLOwner(AActor* QLOwner_ext);
    virtual void UnSetQLOwner();
    AActor* GetQLOwner();

    void PlaySoundComponent(const FName& SoundName);
    void PlaySoundFireAndForget(const FName& SoundName);

    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box component")
    //UBoxComponent* BoxComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    UStaticMeshComponent* StaticMeshComponent;

    UStaticMeshComponent*& GetStaticMeshComponent();
protected:
    AActor* QLOwner;
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
};
