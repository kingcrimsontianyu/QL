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