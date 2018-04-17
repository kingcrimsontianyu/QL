//----------------------------------------
// Quarter Life
//
// GNU General Public License v3.0
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------

#pragma once

// for UUserWidget
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "QLPauseMenuWidget.h"
#include "QLUtility.h"
#include "GameFramework/PlayerController.h"
#include "QLPlayerController.generated.h"

UCLASS()
class QL_API AQLPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AQLPlayerController();
    virtual void SetupInputComponent() override;

    // Sets jump flag when key is pressed.
    UFUNCTION()
    void Pause();

    // reference UMG asset in the editor
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PauseMenuWidget")
    TSubclassOf<UUserWidget> PauseMenuWidget;

    // hold the widget after creation
    UQLPauseMenuWidget* PauseMenu;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    bool GetGamePaused();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void PauseGame();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void ResumeGame();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    float GetControlledPawnVelocityLength() const;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    virtual void Tick(float DeltaSeconds) override;

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    float GetFramePerSecond() const;
private:
    bool bGamePaused;
    float framePerSecond;
};
