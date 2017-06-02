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
#include "QLActor.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLActor::AQLActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    QLOwner = nullptr;
    StaticMeshComponent = nullptr;

    SoundNoAttenuation = CreateDefaultSubobject<USoundAttenuation>(TEXT("SoundNoAttenuation"));
    SoundNoAttenuation->Attenuation.bAttenuate = false;

    SoundAttenuation = CreateDefaultSubobject<USoundAttenuation>(TEXT("SoundAttenuation"));
    SoundAttenuation->Attenuation.bAttenuate = true;
    SoundAttenuation->Attenuation.bSpatialize = true;
    SoundAttenuation->Attenuation.SpatializationAlgorithm = ESoundSpatializationAlgorithm::SPATIALIZATION_Default;
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLActor::BeginPlay()
{
    Super::BeginPlay();
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLActor::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLActor::SetQLOwner(AActor* QLOwner_ext)
{
    QLOwner = QLOwner_ext;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLActor::UnSetQLOwner()
{
    QLOwner = nullptr;
}

//------------------------------------------------------------
//------------------------------------------------------------
AActor* AQLActor::GetQLOwner()
{
    return QLOwner;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLActor::PlaySoundComponent(const FName& SoundName)
{
    QLUtility::PlaySoundComponent(SoundComponentList, SoundName);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLActor::PlaySoundFireAndForget(const FName& SoundName, const FVector& Location)
{
    QLUtility::PlaySoundFireAndForget(this->GetWorld(),
        FireAndForgetSoundWaveList,
        SoundName,
        Location,
        SoundAttenuation);
}

//------------------------------------------------------------
// note: ConstructorHelpers::FObjectFinder<T> and
// CreateDefaultSubobject<T> can only be used inside ctor!!!
//------------------------------------------------------------
UAudioComponent* AQLActor::CreateSoundComponent(USceneComponent*& RootComponent_ext, const TCHAR* soundPath, const TCHAR* soundName)
{
    ConstructorHelpers::FObjectFinder<USoundWave> soundWave(soundPath);
    UAudioComponent* soundComp = CreateDefaultSubobject<UAudioComponent>(soundName);

    bool success = false;
    if (soundWave.Succeeded() && soundComp)
    {
        soundComp->SetSound(soundWave.Object);
        soundComp->SetupAttachment(RootComponent_ext);
        soundComp->SetRelativeLocation(FVector(0.0f));
        soundComp->bAutoActivate = false;
        soundComp->AdjustAttenuation(SoundAttenuation->Attenuation);
        success = true;
    }

    if (!success)
    {
        QLUtility::QLSay(TEXT("AQLActor::CreateSoundComponent() failed."));
        soundComp = nullptr;
    }

    return soundComp;
}

//------------------------------------------------------------
//------------------------------------------------------------
USoundWave* AQLActor::CreateFireAndForgetSoundWave(const TCHAR* SoundPath, const TCHAR* SoundName)
{
    ConstructorHelpers::FObjectFinder<USoundWave> SoundWaveObj(SoundPath);

    if (SoundWaveObj.Succeeded())
    {
        return SoundWaveObj.Object;
    }
    else
    {
        QLUtility::QLSay(TEXT("CreateFireAndForgetSoundWave() failed."));
        return nullptr;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
UStaticMeshComponent*& AQLActor::GetStaticMeshComponent()
{
    return StaticMeshComponent;
}