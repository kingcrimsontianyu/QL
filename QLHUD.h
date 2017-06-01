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
#include "QLCharacter.h"
#include "GameFramework/HUD.h"
#include "QLHUD.generated.h"

UCLASS()
class QL_API AQLHUD : public AHUD
{
    GENERATED_BODY()
public:
    AQLHUD();
    virtual void DrawHUD() override;

    void DrawDefaultCrosshair();

protected:
    TArray<UTexture2D*> CrosshairTextureList;
    UTexture2D* CrosshairTextureDefault;
};
