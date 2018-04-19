// Fill out your copyright notice in the Description page of Project Settings.

#include "QL.h"
#include "QLUtility.h"
#include "QLReLocator.h"
#include "QLCharacter.h"
#include "QLGameModeBase.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLReLocator::AQLReLocator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // built-in dynamic delegate
    this->OnActorBeginOverlap.AddDynamic(this, &AQLReLocator::OnOverlapBeginForActor);
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLReLocator::BeginPlay()
{
	Super::BeginPlay();
	
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLReLocator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLReLocator::OnOverlapBeginForActor(AActor* OverlappedActor, AActor* OtherActor)
{
    AQLCharacter* QLCharacter = Cast<AQLCharacter>(OtherActor);
    if (QLCharacter)
    {
        if (GetWorld())
        {
            AQLGameModeBase* QLGameModeBase = Cast<AQLGameModeBase>(GetWorld()->GetAuthGameMode());
            APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
            if (QLGameModeBase && PlayerController)
            {
                AActor* Result = QLGameModeBase->ChoosePlayerStart(PlayerController);
                if (Result)
                {
                    APlayerStart* PlayerStart = Cast<APlayerStart>(Result);
                    if (PlayerStart)
                    {
                        QLCharacter->SetActorTransform(PlayerStart->GetActorTransform());
                        QLCharacter->GetMovementComponent()->StopMovementImmediately();
                        PlaySoundFireAndForget("Relocate");
                    }                                        
                }
            }
        }
    }
}