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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    UTexture2D* CrosshairTextureDefault;

protected:
    TArray<UTexture2D*> CrosshairTextureList;
};
