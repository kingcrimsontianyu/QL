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
#include "QLGameModeFPS.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLGameModeFPS::AQLGameModeFPS()
{
    static ConstructorHelpers::FObjectFinder<UClass> QLCharacterObj(TEXT("AQLCharacter'/Game/Blueprints/Essential/BP_QLCharacter.BP_QLCharacter_C'"));
    if (QLCharacterObj.Object)
    {
        DefaultPawnClass = QLCharacterObj.Object;
    }

    PlayerControllerClass = AQLPlayerController::StaticClass();
    HUDClass = AQLHUD::StaticClass();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLGameModeFPS::StartPlay()
{
    Super::StartPlay();
}