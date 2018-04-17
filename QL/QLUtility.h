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

namespace QLUtility
{
    void QLSay(const FString& string);
    void QLSayLong(const FString& string);
    void QLSay(const FString& string, const float time);
    void QLWarning(const FString& string);

    void PlaySoundComponent(TMap<FName, UAudioComponent*>& SoundComponentList, const FName& SoundName);
    void PlaySoundFireAndForget(UWorld* World,
        TMap<FName, USoundWave*>& FireAndForgetSoundList,
        const FName& SoundName,
        const FVector& Location,
        USoundAttenuation* SoundAttenuation);
}
