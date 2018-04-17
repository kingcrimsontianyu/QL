//----------------------------------------
// Quarter Life
//
// MIT license
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------

#pragma once

#include "QLUtility.h"
#include "Blueprint/UserWidget.h"
#include "QLPlayGameMenuWidget.generated.h"

UCLASS()
class QL_API UQLPlayGameMenuWidget : public UUserWidget
{
public:
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "C++Function")
    virtual void Play();

    UFUNCTION(BlueprintCallable, Category = "C++Function")
    void SetPlayerController(AQLPlayerController* PlayerController_ext);

private:
    AQLPlayerController* PlayerController;
};
