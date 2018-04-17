//----------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------

#include "QL.h"
#include "QLGameInstance.h"

//------------------------------------------------------------
//------------------------------------------------------------
FSoundResult::FSoundResult() :
    bSuccess(false),
    bNew(false),
    SoundWave(nullptr)
{
}

//------------------------------------------------------------
//------------------------------------------------------------
UQLGameInstance::UQLGameInstance() :
    PersistentData()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
FSoundResult UQLGameInstance::AddToSoundWaveList(TAssetPtr<USoundWave> SoundWaveAsset)
{
    FSoundResult Result;
    USoundWave** pSoundWave = PersistentData.SoundWaveList.Find(SoundWaveAsset);

    // if asset is found, retrieve the loaded sound
    if (pSoundWave)
    {
        Result.SoundWave = *pSoundWave;
    }
    // if asset is not found, load the sound
    else
    {
        bool bManageActiveHandle = true;
        UObject* ResultObj = PersistentData.StreamableManager.SynchronousLoad(SoundWaveAsset.ToStringReference());
        if (ResultObj)
        {
            Result.SoundWave = Cast<USoundWave>(ResultObj);
            if (Result.SoundWave)
            {
                Result.bNew = true;
                PersistentData.SoundWaveList.Add(SoundWaveAsset, Result.SoundWave);
            }
        }
    }

    return Result;
}

//------------------------------------------------------------
//------------------------------------------------------------
FSoundResult UQLGameInstance::GetSoundWaveFromList(TAssetPtr<USoundWave> SoundWaveAsset)
{
    FSoundResult Result;
    USoundWave** pSoundWave = PersistentData.SoundWaveList.Find(SoundWaveAsset);

    // if asset is found, retrieve the loaded sound
    if (pSoundWave)
    {
        Result.bSuccess = true;
        Result.SoundWave = *pSoundWave;
    }

    return Result;
}

//------------------------------------------------------------
//------------------------------------------------------------
QLPersistentData::QLPersistentData() :
    StreamableManager()
{
}