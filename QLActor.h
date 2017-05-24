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
    virtual void SetQLOwner(AActor* QLOwner);
    virtual void UnSetQLOwner();
    AActor* GetQLOwner();

    void PlaySoundComponent(const FName& SoundName);
    void PlaySoundFireAndForget(const FName& SoundName, const FVector& Location);

    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box component")
    //UBoxComponent* BoxComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static mesh component")
    UStaticMeshComponent* StaticMeshComponent;

    UStaticMeshComponent*& GetStaticMeshComponent();
protected:
    AActor* QLOwner;
    USoundAttenuation* SoundNoAttenuation;
    USoundAttenuation* SoundAttenuation;
    TMap<FName, UAudioComponent*> SoundComponentList;
    TMap<FName, USoundWave*> FireAndForgetSoundWaveList;
    UAudioComponent* CreateSoundComponent(USceneComponent*& RootComponent, const TCHAR* soundPath, const TCHAR* soundName);
    USoundWave* CreateFireAndForgetSoundWave(const TCHAR* SoundPath, const TCHAR* SoundName);
};
