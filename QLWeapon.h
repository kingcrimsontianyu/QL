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

#include "QLActor.h"
#include "GameFramework/Character.h"
#include "QLWeapon.generated.h"

// forward declaration
class AQLCharacter;

UCLASS()
class QL_API AQLWeapon : public AQLActor
{
    GENERATED_BODY()
public:
    // Sets default values for this actor's properties
    AQLWeapon();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick( float DeltaSeconds ) override;

    virtual void Fire() {};
    virtual void FireReleased() {};
    virtual void FireRepeat() {};

    virtual void AltFire() {};
    virtual void AltFireReleased() {};
    virtual void AltFireRepeat() {};

    void AddActiveCrosshairTexture(const FName& Name_ext);
    UTexture2D* CreateCrosshairTexture(const TCHAR* texturePath);
    const FName& GetWeaponName() const;

    UFUNCTION()
    void OnOverlapBeginForActor(AActor* OverlappedActor, AActor* OtherActor);

    virtual void SetQLOwner(AActor* QLOwner_ext) override;
    virtual void UnSetQLOwner() override;
    AQLCharacter* GetWeaponOwner();
    virtual void ResetWeapon() {};

    virtual void Zoom() {};

    const TArray<UTexture2D*>& GetActiveCrosshairTextureList() const;

    UTexture2D* GetCrosshairTexture(const FName& Name_ext) const;

    void ReplaceActiveCrosshairTexture(UTexture2D* CrosshairTexture, int Index);
protected:
    FName Name;
    bool bIsFireHeldDown;
    bool bIsAltFireHeldDown; // key/button is held down for some time
    bool bIsAltFirePressed; // key/button is pressed, regardless if it is subsequently released or held down
    AQLCharacter* WeaponOwner;
    FHitResult Hit;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++Property")
    TMap<FName, UTexture2D*> CrosshairTextureList;

    TArray<UTexture2D*> ActiveCrosshairTextureList;
};
