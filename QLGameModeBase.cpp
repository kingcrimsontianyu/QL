//----------------------------------------
// Quarter Life
//
// MIT license
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------

#include "QL.h"
#include "QLGameModeBase.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLGameModeBase::AQLGameModeBase() :
    PersistentData()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLGameModeBase::StartPlay()
{
    Super::StartPlay();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLGameModeBase::AddFireAndForgetSoundWaveToList(FName SoundName, TAssetPtr<USoundWave> SoundWaveAsset)
{
    QLSoundBlob& Value = PersistentData.FireAndForgetSoundList.Add(SoundName, QLSoundBlob(SoundWaveAsset));
    UObject* Result = PersistentData.StreamableManager.SynchronousLoad(Value.SoundWaveAsset.ToStringReference());

    bool bLoadSuccessful = false;
    if (Result)
    {
        USoundWave* SoundWave = Cast<USoundWave>(Result);
        if (SoundWave)
        {
            bLoadSuccessful = true;
            Value.SoundWave = SoundWave;
        }
    }

    if (!bLoadSuccessful)
    {
        QLUtility::QLSay("AddFireAndForgetSoundWaveToList() failed.");
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLGameModeBase::PlaySoundFireAndForget(const FName& SoundName)
{
    QLSoundBlob* p = PersistentData.FireAndForgetSoundList.Find(SoundName);
    if (p)
    {
        USoundWave* SoundWave = (*p).SoundWave;
        if (SoundWave)
        {
            UGameplayStatics::PlaySound2D(GetWorld(), SoundWave);
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
QLSoundBlob::QLSoundBlob(TAssetPtr<USoundWave> SoundWaveAsset_ext) :
    SoundWaveAsset(SoundWaveAsset_ext),
    SoundWave(nullptr)
{
}

//------------------------------------------------------------
//------------------------------------------------------------
QLPersistentData::QLPersistentData() :
    StreamableManager()
{
}