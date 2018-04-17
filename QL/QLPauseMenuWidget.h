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

#include "QLUtility.h"
#include "Blueprint/UserWidget.h"
#include "QLPauseMenuWidget.generated.h"

// Note:
// 1. BP UMG widget cannot be directly referenced and used in C++. Solution:
//    (1) BP player controller is derived from c++ counterpart
//        with a public member pointer that points to a UUserWidget object.
//    (2) BP game mode is derived from c++ counterpart.
//        BP player controller is assigned to that BP game mode.
//        BP UMG widget is assigned to that BP player controller.
// 2. At runtime there is a warning that indicates CreateWidget() can only be
//    applied to UUserWidget's subclass instead of UUserWidget. Solution:
//    (1) Create an empty C++ user widget as the subclass.
//    (2) Re-parent that BP UMG widget accordingly.

// forward declaration
class AQLPlayerController;

UCLASS()
class QL_API UQLPauseMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    virtual void ResumeGame();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    virtual void SaveGame();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    virtual void ReturnToMainMenu();

    void SetPlayerController(AQLPlayerController* PlayerController_ext);
private:
    AQLPlayerController* PlayerController;
};
