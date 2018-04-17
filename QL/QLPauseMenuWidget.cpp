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
#include "QLPlayerController.h"
#include "QLPauseMenuWidget.h"

//------------------------------------------------------------
//------------------------------------------------------------
void UQLPauseMenuWidget::ResumeGame()
{
    if (PlayerController)
    {
        PlayerController->ResumeGame();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLPauseMenuWidget::SaveGame()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLPauseMenuWidget::ReturnToMainMenu()
{
    UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("QLMainMenu")));
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLPauseMenuWidget::SetPlayerController(AQLPlayerController* PlayerController_ext)
{
    this->PlayerController = PlayerController_ext;
}