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

#include "Engine/GameInstance.h"
#include "QLGameInstance.generated.h"

//------------------------------------------------------------
//------------------------------------------------------------
USTRUCT(BlueprintType)
struct FSoundResult
{
    GENERATED_BODY()

    FSoundResult();

    UPROPERTY()
    bool bSuccess;

    UPROPERTY()
    bool bNew;

    UPROPERTY()
    USoundWave* SoundWave;
};

//------------------------------------------------------------
//------------------------------------------------------------
class QLPersistentData
{
public:
    QLPersistentData();
    FStreamableManager StreamableManager;
    TMap<TAssetPtr<USoundWave>, USoundWave*> SoundWaveList;
};

//------------------------------------------------------------
//------------------------------------------------------------
UCLASS()
class QL_API UQLGameInstance : public UGameInstance
{
    GENERATED_BODY()
public:
    UQLGameInstance();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    FSoundResult AddToSoundWaveList(TAssetPtr<USoundWave> SoundWaveAsset);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    FSoundResult GetSoundWaveFromList(TAssetPtr<USoundWave> SoundWaveAsset);
protected:
    QLPersistentData PersistentData;
};
