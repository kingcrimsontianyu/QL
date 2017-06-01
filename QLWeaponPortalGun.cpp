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
#include "QLWeaponPortalGun.h"
#include "QLCharacter.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLWeaponPortalGun::AQLWeaponPortalGun()
{
    PrimaryActorTick.bCanEverTick = true;
    Name = "PortalGun";
    CrosshairTextureList.Add("BlueEmpty", CreateCrosshairTexture(TEXT("/Game/Blueprints/Weapon/PortalGun/Internal/portal_gun_crosshair_blue_empty_processed")));
    CrosshairTextureList.Add("BlueFilled", CreateCrosshairTexture(TEXT("/Game/Blueprints/Weapon/PortalGun/Internal/portal_gun_crosshair_blue_filled_processed")));
    CrosshairTextureList.Add("OrangeEmpty", CreateCrosshairTexture(TEXT("/Game/Blueprints/Weapon/PortalGun/Internal/portal_gun_crosshair_orange_empty_processed")));
    CrosshairTextureList.Add("OrangeFilled", CreateCrosshairTexture(TEXT("/Game/Blueprints/Weapon/PortalGun/Internal/portal_gun_crosshair_orange_filled_processed")));

    InitializeCurrentCrosshairTexture("BlueEmpty");
    InitializeCurrentCrosshairTexture("OrangeEmpty");

    // sound
    SoundComponentList.Add("None", CreateSoundComponent(RootComponent, TEXT("/Game/Sounds/bottle"), TEXT("SoundNoneComp")));
    SoundComponentList.Add("Fire", CreateSoundComponent(RootComponent, TEXT("/Game/Sounds/portal_created"), TEXT("SoundFireComp")));
    SoundComponentList.Add("Zoom", CreateSoundComponent(RootComponent, TEXT("/Game/Sounds/zoom_in"), TEXT("SoundZoomComp")));

    BluePortal = nullptr;
    OrangePortal = nullptr;

    // zoom
    static ConstructorHelpers::FObjectFinder<UCurveFloat> FCurveObj(TEXT("/Game/Blueprints/Weapon/C_QLZoomCurve"));
    if (FCurveObj.Object)
    {
        FCurve = FCurveObj.Object;
    }
    ZoomTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ZoomTimeline"));
    ZoomTimelineInterpFunction.BindUFunction(this, FName{TEXT("ZoomCallback")});
    bCanZoomIn = true;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponPortalGun::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    ZoomTimeline->AddInterpFloat(FCurve, ZoomTimelineInterpFunction, FName{ TEXT("Zoom") });
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponPortalGun::Fire()
{
    CreatePortal(EPortalType::Blue);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponPortalGun::AltFire()
{
    CreatePortal(EPortalType::Orange);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponPortalGun::CreatePortal(EPortalType PortalType)
{
    Hit = WeaponOwner->RayTraceFromCharacterPOV();
    // if hit occurs
    if (Hit.bBlockingHit)
    {
        // check if the hit actor compatible with (i.e. responsive to) portal gun
        AQLPortalGunCompatibleActor* pgcActor = Cast<AQLPortalGunCompatibleActor>(Hit.GetActor());
        if (pgcActor)
        {
            // remove portal of the same color that is generated elsewhere
            if (PortalType == EPortalType::Blue)
            {
                if (BluePortal)
                {
                    BluePortal->UnsetPortal();
                    BluePortal->Destroy();
                    BluePortal = nullptr;
                }
            }
            else
            {
                if (OrangePortal)
                {
                    OrangePortal->UnsetPortal();
                    OrangePortal->Destroy();
                    OrangePortal = nullptr;
                }
            }

            // defer-spawn portal
            // the portal should be located outside the portal gun compatible actor
            // and these two actors should have an almost coincident surface
            FVector location = Hit.ImpactPoint;
            FRotator rotation = UKismetMathLibrary::MakeRotFromXZ(Hit.Normal, pgcActor->GetActorUpVector());
            FTransform transform(FRotator::ZeroRotator);

            AQLPortal* Portal = GetWorld()->SpawnActorDeferred<AQLPortal>(AQLPortal::StaticClass(), transform);
            Portal->SetQLOwner(this);
            // budge the portal so that it only overlaps the wall a wee bit
            // to allow decal display
            location += (Portal->GetBoxComponent()->GetUnscaledBoxExtent().X - 0.05f) * Hit.Normal;
            Portal->SetActorLocation(location);
            Portal->SetActorRotation(rotation);
            UGameplayStatics::FinishSpawningActor(Portal, transform);

            // the newly spawned portal has top priority
            // previously spawned portal is now destroyed
            TSet<AActor*> AActorList;
            Portal->GetOverlappingActors(AActorList, AQLPortal::StaticClass());
            if (AActorList.Num() > 0)
            {
                for (auto& Item : AActorList)
                {
                    if (Item == BluePortal)
                    {
                        BluePortal->UnsetPortal();
                        BluePortal = nullptr;
                    }
                    else
                    {
                        OrangePortal->UnsetPortal();
                        OrangePortal = nullptr;
                    }
                    Item->Destroy();
                }
            }

            // now that a new portal is appropriately created without overlap,
            // set the new portal's properties
            // esp, set spouse and set spouse's spouse
            if (PortalType == EPortalType::Blue)
            {
                Portal->SetPortal(EPortalType::Blue, OrangePortal);
                BluePortal = Portal;
            }
            else
            {
                Portal->SetPortal(EPortalType::Orange, BluePortal);
                OrangePortal = Portal;
            }

            // apply sound
            PlaySoundComponent("Fire");
        }
        else
        {
            // apply sound
            PlaySoundComponent("None");
        }
    }
    else
    {
        // apply sound
        PlaySoundComponent("None");
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponPortalGun::Tick(float DeltaSeconds)
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponPortalGun::ResetWeapon()
{
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponPortalGun::QueryPortal()
{
    FString BluePortalStatus = BluePortal ? "blue portal on" : "blue portal off";
    FString OrangePortalStatus = OrangePortal ? "orange portal on" : "orange portal off";
    QLUtility::QLSay(BluePortalStatus + "     " + OrangePortalStatus);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponPortalGun::Zoom()
{
    PlaySoundComponent("Zoom");

    if (bCanZoomIn)
    {
        ZoomTimeline->Play();
        bCanZoomIn = false;
    }
    else
    {
        ZoomTimeline->Reverse();
        bCanZoomIn = true;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponPortalGun::ZoomCallback(float Val)
{
    if (WeaponOwner)
    {
        APlayerCameraManager* cm = UGameplayStatics::GetPlayerCameraManager(WeaponOwner->GetWorld(), 0);
        cm->SetFOV(Val);
    }
}