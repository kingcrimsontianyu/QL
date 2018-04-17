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
#include "QLPlayerController.h"
#include "QLPlayGameMenuWidget.h"

//------------------------------------------------------------
//------------------------------------------------------------
void UQLPlayGameMenuWidget::Play()
{
    if (PlayerController)
    {
        PlayerController->bShowMouseCursor = false;
        FInputModeGameOnly InputMode;
        PlayerController->SetInputMode(InputMode);
        PlayerController->SetPause(false);

        UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("QLArena")));
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void UQLPlayGameMenuWidget::SetPlayerController(AQLPlayerController* PlayerController_ext)
{
    this->PlayerController = PlayerController_ext;
}