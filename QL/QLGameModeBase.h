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

#include "QLCameraPawn.h"
#include "QLCharacter.h"
#include "QLPlayerController.h"
#include "QLHUD.h"
#include "GameFramework/GameMode.h"
#include "QLGameModeBase.generated.h"

UCLASS()
class QL_API AQLGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    virtual void StartPlay() override;
};
