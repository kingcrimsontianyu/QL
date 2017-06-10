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

namespace QLUtility
{
    void QLSay(const FString& string);
    void QLSayLong(const FString& string);
    void QLSay(const FString& string, const float time);

    void PlaySoundComponent(TMap<FName, UAudioComponent*>& SoundComponentList, const FName& SoundName);
    void PlaySoundFireAndForget(UWorld* World,
        TMap<FName, USoundWave*>& FireAndForgetSoundList,
        const FName& SoundName,
        const FVector& Location,
        USoundAttenuation* SoundAttenuation);
}
