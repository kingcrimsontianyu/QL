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
#include "QLGameModeMainMenu.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLGameModeMainMenu::AQLGameModeMainMenu()
{
    DefaultPawnClass = AQLCameraPawn::StaticClass();
    PlayerControllerClass = AQLPlayerController::StaticClass();
    HUDClass = AQLHUD::StaticClass();
}