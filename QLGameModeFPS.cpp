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
    DefaultPawnClass = AQLCharacter::StaticClass();
    PlayerControllerClass = AQLPlayerController::StaticClass();
    HUDClass = AQLHUD::StaticClass();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLGameModeFPS::StartPlay()
{
    Super::StartPlay();
}