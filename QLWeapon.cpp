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
#include "QLWeapon.h"
#include "QLCharacter.h"

//------------------------------------------------------------
// Sets default values
//------------------------------------------------------------
AQLWeapon::AQLWeapon()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Name = "None";
    bIsFireHeldDown = false;
    bIsAltFireHeldDown = false;
    bIsAltFirePressed = false;

    //RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

    // built-in dynamic delegate
    this->OnActorBeginOverlap.AddDynamic(this, &AQLWeapon::OnOverlapBeginForActor);
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLWeapon::BeginPlay()
{
    Super::BeginPlay();
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLWeapon::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::AddActiveCrosshairTexture(const FName& Name)
{
    bool bSuccess = false;
    UTexture2D** valuePtr = CrosshairTextureList.Find(Name);
    if (valuePtr)
    {
        UTexture2D* crosshairTexture = *valuePtr;
        if (crosshairTexture)
        {
            ActiveCrosshairTextureList.Add(crosshairTexture);
            bSuccess = true;
        }
    }

    if(!bSuccess)
    {
        QLUtility::QLWarning("crosshair not found.");
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
UTexture2D* AQLWeapon::CreateCrosshairTexture(const TCHAR* texturePath)
{
    ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTextureObj(texturePath);
    UTexture2D* CrosshairTexture = CrosshairTextureObj.Object;

    if(CrosshairTextureObj.Object->IsValidLowLevel() && CrosshairTexture)
    {
        return CrosshairTexture;
    }
    else
    {
        QLUtility::QLSay(TEXT("AQLWeapon::CreateCrosshairTexture() failed."));
        return nullptr;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
const FName& AQLWeapon::GetWeaponName() const
{
    return Name;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::OnOverlapBeginForActor(AActor* OverlappedActor, AActor* OtherActor)
{
    AQLCharacter* player = Cast<AQLCharacter>(OtherActor);
    // if the overlapping actor is player, equip him with the weapon
    if (player)
    {
        player->PickUpWeapon(this);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::SetQLOwner(AActor* QLOwner)
{
    Super::SetQLOwner(QLOwner);
    this->WeaponOwner = Cast<AQLCharacter>(QLOwner);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::UnSetQLOwner()
{
    Super::SetQLOwner(nullptr);
    this->WeaponOwner = nullptr;
}

//------------------------------------------------------------
//------------------------------------------------------------
AQLCharacter* AQLWeapon::GetWeaponOwner()
{
    return WeaponOwner;
}

//------------------------------------------------------------
//------------------------------------------------------------
const TArray<UTexture2D*>& AQLWeapon::GetActiveCrosshairTextureList() const
{
    return ActiveCrosshairTextureList;
}

//------------------------------------------------------------
//------------------------------------------------------------
UTexture2D* AQLWeapon::GetCrosshairTexture(const FName& Name) const
{
    UTexture2D* const* pTexture = CrosshairTextureList.Find(Name);
    if (pTexture)
    {
        return *pTexture;
    }
    else
    {
        return nullptr;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLWeapon::ReplaceActiveCrosshairTexture(UTexture2D* CrosshairTexture, int Index)
{
    if (CrosshairTexture && Index >= 0 && Index < ActiveCrosshairTextureList.Num())
    {
        ActiveCrosshairTextureList[Index] = CrosshairTexture;
    }
}