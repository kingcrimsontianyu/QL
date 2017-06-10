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
#include "QLHUD.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLHUD::AQLHUD()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLHUD::DrawHUD()
{
    Super::DrawHUD();

    AQLCharacter* QLCharacter = Cast<AQLCharacter>(GetOwningPawn());
    // if owning character exists
    if (QLCharacter)
    {
        AQLWeapon* CurrentWeapon = QLCharacter->GetCurrentWeapon();
        // if owning character's weapon exists
        if (CurrentWeapon)
        {
            CrosshairTextureList = CurrentWeapon->CurrentCrosshairTextureList;
            // if owning pawn's weapon' crosshair exists
            if (CrosshairTextureList.Num())
            {
                for (int32 i = 0; i < CrosshairTextureList.Num(); ++i)
                {
                    // Find the center of our canvas.
                    FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

                    // Offset by half of the texture's dimensions so that the center of the texture aligns with the center of the Canvas.
                    FVector2D CrossHairDrawPosition(Center.X - (CrosshairTextureList[i]->GetSurfaceWidth() * 0.5f), Center.Y - (CrosshairTextureList[i]->GetSurfaceHeight() * 0.5f));

                    // Draw the crosshair at the centerpoint.
                    FCanvasTileItem TileItem(CrossHairDrawPosition, CrosshairTextureList[i]->Resource, FLinearColor::White);
                    TileItem.BlendMode = SE_BLEND_Translucent;
                    Canvas->DrawItem(TileItem);
                }
            }
            // if weapon does not have specific cross-hair, use default
            else
            {
                DrawDefaultCrosshair();
            }
        }
        // if no owning character, use the default cross-hair
        else
        {
            DrawDefaultCrosshair();
        }
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLHUD::DrawDefaultCrosshair()
{
    // Find the center of our canvas.
    FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

    // Offset by half of the texture's dimensions so that the center of the texture aligns with the center of the Canvas.
    FVector2D CrossHairDrawPosition(Center.X - (CrosshairTextureDefault->GetSurfaceWidth() * 0.5f), Center.Y - (CrosshairTextureDefault->GetSurfaceHeight() * 0.5f));

    // Draw the crosshair at the centerpoint.
    FCanvasTileItem TileItem(CrossHairDrawPosition, CrosshairTextureDefault->Resource, FLinearColor::White);
    TileItem.BlendMode = SE_BLEND_Translucent;
    Canvas->DrawItem(TileItem);
}