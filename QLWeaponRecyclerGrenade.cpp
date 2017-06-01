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
#include "QLWeaponRecyclerGrenade.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLWeaponRecyclerGrenade::AQLWeaponRecyclerGrenade()
{
    PrimaryActorTick.bCanEverTick = true;
    NumRecyclerGrenade = 0;
    Name = "RecyclerGrenade";

    CrosshairTextureList.Add("Regular", CreateCrosshairTexture(TEXT("/Game/Blueprints/Weapon/GravityGun/Crosshair/gravity_gun_crosshair")));
    InitializeCurrentCrosshairTexture("Regular");

    // sound
    //SoundComponentList.Add("None", CreateSoundComponent(RootComponent, TEXT("/Game/Sounds/bottle"), TEXT("SoundNoneComp")));
    //FireAndForgetSoundWaveList.Add("Start", CreateFireAndForgetSoundWave(TEXT("/Game/Sounds/za_warudo"), TEXT("SoundCompStart")));
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLWeaponRecyclerGrenade::BeginPlay()
{
    Super::BeginPlay();
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLWeaponRecyclerGrenade::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeaponRecyclerGrenade::Fire()
{
}