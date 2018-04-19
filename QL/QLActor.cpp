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
#include "QLUtility.h"
#include "QLActor.h"
#include "QLGameModeBase.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLActor::AQLActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    QLOwner = nullptr;
    StaticMeshComponent = nullptr;

    //SoundNoAttenuation = CreateDefaultSubobject<USoundAttenuation>(TEXT("SoundNoAttenuation"));
    //SoundNoAttenuation->Attenuation.bAttenuate = false;

    //SoundAttenuation = CreateDefaultSubobject<USoundAttenuation>(TEXT("SoundAttenuation"));
    //SoundAttenuation->Attenuation.bAttenuate = true;
    //SoundAttenuation->Attenuation.bSpatialize = true;
    //SoundAttenuation->Attenuation.SpatializationAlgorithm = ESoundSpatializationAlgorithm::SPATIALIZATION_Default;
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLActor::BeginPlay()
{
    Super::BeginPlay();

    for (auto It = SoundComponentAssetList.CreateConstIterator(); It; ++It)
    {
        CreateSoundComponent(It->Key, It->Value);
    }

    for (auto It = FireAndForgetSoundAssetList.CreateConstIterator(); It; ++It)
    {
        CreateFireAndForgetSound(It->Key, It->Value);
    }
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
void AQLActor::PlaySoundFireAndForget(const FName& SoundName)
{
    QLUtility::PlaySoundFireAndForget(this->GetWorld(),
        FireAndForgetSoundList,
        SoundName,
        GetActorLocation(),
        SoundAttenuation);
}

//------------------------------------------------------------
//------------------------------------------------------------
UAudioComponent* AQLActor::CreateSoundComponent(FName SoundName, TAssetPtr<USoundWave> SoundWaveAsset)
{
    FSoundResult Result;
    UQLGameInstance* QLGameInstance = Cast<UQLGameInstance>(GetGameInstance());
    if (QLGameInstance)
    {
        Result = QLGameInstance->AddToSoundWaveList(SoundWaveAsset);
    }

    UAudioComponent* SoundComp = NewObject<UAudioComponent>(this, UAudioComponent::StaticClass());

    if (Result.SoundWave && SoundComp)
    {
        SoundComp->SetSound(Result.SoundWave);
        SoundComp->SetupAttachment(RootComponent);
        SoundComp->SetRelativeLocation(FVector(0.0f));
        SoundComp->bAutoActivate = false;
        //SoundComp->AdjustAttenuation(SoundAttenuation->Attenuation);
        SoundComp->RegisterComponent(); // must occur after audio component is properly set up

        SoundComponentList.Add(SoundName, SoundComp);
    }

    if (!Result.bNew)
    {
        // QLUtility::QLSay("Asset of " + SoundName.ToString() + " is not new.");
    }

    return SoundComp;
}

//------------------------------------------------------------
//------------------------------------------------------------
USoundWave* AQLActor::CreateFireAndForgetSound(FName SoundName, TAssetPtr<USoundWave> SoundWaveAsset)
{
    FSoundResult Result;
    UQLGameInstance* QLGameInstance = Cast<UQLGameInstance>(GetGameInstance());
    if (QLGameInstance)
    {
        Result = QLGameInstance->AddToSoundWaveList(SoundWaveAsset);
    }

    if (Result.SoundWave)
    {
        FireAndForgetSoundList.Add(SoundName, Result.SoundWave);
    }

    if (!Result.bNew)
    {
        // QLUtility::QLSay("Asset of " + SoundName.ToString() + " is not new.");
    }

    return Result.SoundWave;
}

//------------------------------------------------------------
//------------------------------------------------------------
UStaticMeshComponent*& AQLActor::GetStaticMeshComponent()
{
    return StaticMeshComponent;
}