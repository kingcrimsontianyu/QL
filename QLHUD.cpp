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
    ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTextureObj(TEXT("/Game/Blueprints/PlayerUMG/Internal/default_crosshair"));
    CrosshairTextureDefault = CrosshairTextureObj.Object;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLHUD::DrawHUD()
{
    Super::DrawHUD();

    DrawWeaponCrosshairIfAny();
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLHUD::DrawWeaponCrosshairIfAny()
{
    AQLCharacter* QLCharacter = Cast<AQLCharacter>(GetOwningPawn());
    // if owning pawn exists
    if (QLCharacter)
    {
        AQLWeapon* CurrentWeapon = QLCharacter->GetCurrentWeapon();
        // if owning pawn's weapon exists
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
        }
        // use the default cross-hair
        else
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
    }
}