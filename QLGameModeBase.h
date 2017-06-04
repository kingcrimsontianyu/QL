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

#include "QLCameraPawn.h"
#include "QLCharacter.h"
#include "QLPlayerController.h"
#include "QLHUD.h"
#include "QLUtility.h"
#include "GameFramework/GameMode.h"
#include "QLGameModeBase.generated.h"

class QLSoundBlob
{
public:
    QLSoundBlob(TAssetPtr<USoundWave> SoundWaveAsset_ext);
    TAssetPtr<USoundWave> SoundWaveAsset;
    USoundWave* SoundWave;
};

class QLPersistentData
{
public:
    QLPersistentData();
    TMap<FName, QLSoundBlob> FireAndForgetSoundList;
    FStreamableManager StreamableManager;
};

UCLASS()
class QL_API AQLGameModeBase : public AGameMode
{
    GENERATED_BODY()

public:
    AQLGameModeBase();
    virtual void StartPlay() override;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void AddFireAndForgetSoundWaveToList(FName SoundName, TAssetPtr<USoundWave> SoundWaveAsset);

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void PlaySoundFireAndForget(const FName& SoundName);
protected:

    QLPersistentData PersistentData;
};
