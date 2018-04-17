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
#include "QLUtility.h"

namespace QLUtility
{
    //------------------------------------------------------------
    //------------------------------------------------------------
    void QLSay(const FString& string)
    {
        QLSay(string, 5.0f);
    }

    //------------------------------------------------------------
    //------------------------------------------------------------
    void QLSayLong(const FString& string)
    {
        QLSay(string, 600.0f);
    }

    //------------------------------------------------------------
    //------------------------------------------------------------
    void QLSay(const FString& string, const float time)
    {
        GEngine->AddOnScreenDebugMessage(-1, time, FColor::Cyan, string);
    }

    //------------------------------------------------------------
    //------------------------------------------------------------
    void QLWarning(const FString& string)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, string);
    }

    //------------------------------------------------------------
    //------------------------------------------------------------
    void PlaySoundComponent(TMap<FName, UAudioComponent*>& SoundComponentList, const FName& SoundName)
    {
        bool Found = false;
        if (SoundComponentList.Contains(SoundName))
        {
            UAudioComponent* SoundComp = SoundComponentList[SoundName];

            // allow sound pile-up
            if (SoundComp)
            {
                Found = true;
                QLUtility::QLSay(SoundName.ToString());
                SoundComp->Play(0.0f);
            }
        }

        if (!Found)
        {
            QLUtility::QLWarning("sound not found.");
        }
    }

    //------------------------------------------------------------
    // FIXME: play sound at location using gravity gun may crash the game
    //------------------------------------------------------------
    void PlaySoundFireAndForget(UWorld* World,
                                TMap<FName, USoundWave*>& FireAndForgetSoundList,
                                const FName& SoundName,
                                const FVector& Location,
                                USoundAttenuation* SoundAttenuation)
    {
        bool Found = false;
        if (FireAndForgetSoundList.Contains(SoundName))
        {
            USoundWave* SoundWave = FireAndForgetSoundList[SoundName];

            // allow sound pile-up
            if (SoundWave)
            {
                Found = true;
                QLUtility::QLSay(SoundName.ToString());

                // FIXME: ATTENUATION CAUSES THE GAME TO CRASH RANDOMLY
                //UGameplayStatics::PlaySoundAtLocation(World,
                //    SoundWave,
                //    Location,
                //    FRotator::ZeroRotator,
                //    1.0f,
                //    1.0f,
                //    0.0f,
                //    SoundAttenuation);

                UGameplayStatics::PlaySound2D(World, SoundWave);
            }
        }

        if (!Found)
        {
            QLUtility::QLWarning("sound not found.");
        }
    }
}