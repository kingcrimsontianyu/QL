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
#include "Engine/GameInstance.h"
#include "QLGameInstance.generated.h"

//------------------------------------------------------------
//------------------------------------------------------------
USTRUCT()
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
