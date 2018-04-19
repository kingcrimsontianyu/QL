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
#include "QLPlayerController.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLPlayerController::AQLPlayerController()
{
    bGamePaused = false;
    PauseMenuWidget = nullptr;
    PauseMenu = nullptr;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPlayerController::Tick(float DeltaSeconds)
{
    framePerSecond = 1.0 / DeltaSeconds;
}

//------------------------------------------------------------
//------------------------------------------------------------
float AQLPlayerController::GetFramePerSecond() const
{
    return framePerSecond;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    FInputActionBinding& PauseToggle = InputComponent->BindAction("Pause", EInputEvent::IE_Pressed, this, &AQLPlayerController::Pause);
    PauseToggle.bExecuteWhenPaused = true;
}

//------------------------------------------------------------
// Design requirement:
// To resume the game, either click umg's resume button,
// or press pause key again. umg handles the first case,
// c++ handles the second case.
//
// note that removing umg from viewport does not mean
// release it from memory.
//------------------------------------------------------------
void AQLPlayerController::Pause()
{
    // pause the game
    if (!bGamePaused)
    {
        // only create once in the life cycle of player controller
        if (!PauseMenu)
        {
            // create the pause menu
            PauseMenu = CreateWidget<UQLPauseMenuWidget>(GetWorld(), PauseMenuWidget);
            PauseMenu->SetPlayerController(this);
        }

        PauseGame();
    }
     // resume the game
    else
    {
        ResumeGame();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
bool AQLPlayerController::GetGamePaused()
{
    return bGamePaused;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPlayerController::PauseGame()
{
    // add the pause menu
    if (PauseMenu)
    {
        bGamePaused = true;
        bShowMouseCursor = true;
        FInputModeGameAndUI InputMode;
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
        SetInputMode(InputMode);
        this->SetPause(true);

        PauseMenu->AddToViewport();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPlayerController::ResumeGame()
{
    // remove the pause menu
    if (PauseMenu)
    {
        bGamePaused = false;
        bShowMouseCursor = false;
        FInputModeGameOnly InputMode;
        SetInputMode(InputMode);
        this->SetPause(false);

        PauseMenu->RemoveFromViewport();
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
float AQLPlayerController::GetControlledPawnVelocityLength() const
{
    APawn* ControlledPawn = GetPawn();
    if (ControlledPawn)
    {
        // !!! note that the pawn must have movement component in order to calculate velocity
        // !!! otherwise the velocity is always zero.
        // !!! todo: for super power mimic matter, the pawn does not use movement component by design
        // !!! how to obtain the velocity then?
        FVector temp = ControlledPawn->GetVelocity();
        return temp.Size();
    }
    else
    {
        return 0.0;
    }
}