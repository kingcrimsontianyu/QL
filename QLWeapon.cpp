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
void AQLWeapon::InitializeCurrentCrosshairTexture(const FName& crosshairTextureName)
{
    UTexture2D** valuePtr = CrosshairTextureList.Find(crosshairTextureName);
    if (valuePtr)
    {
        UTexture2D* crosshairTexture = *valuePtr;
        if (crosshairTexture)
        {
            CurrentCrosshairTextureList.Add(crosshairTexture);
        }
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