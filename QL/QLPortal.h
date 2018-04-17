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

#include "QLActor.h"
#include "QLPortal.generated.h"

// forward declaration
class AQLWeaponPortalGun;

UENUM()
enum class EPortalType : uint8
{
    Blue,
    Orange,
};

// portal owner is the portal gun
UCLASS()
class QL_API AQLPortal : public AQLActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQLPortal();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    UFUNCTION()
    void OnOverlapBeginForActor(AActor* OverlappedActor, AActor* OtherActor);

    UFUNCTION()
    void OnOverlapEndForActor(AActor* OverlappedActor, AActor* OtherActor);

    virtual void SetQLOwner(AActor* QLOwner) override;
    virtual void UnSetQLOwner() override;
    AQLWeaponPortalGun* GetPortalOwner();

    void SetSpouse(AQLPortal* Spouse);
    AQLPortal* GetSpouse();
    void QueryPortal();
    void AddToRoll(AActor* Actor);
    void RemoveFromRoll(AActor* Actor);
    bool IsInMyRoll(AActor* Actor);

    void SetPortal(EPortalType PortalType, AQLPortal* Spouse);
    void UnsetPortal();

    UBoxComponent*& GetBoxComponent();
protected:
    UBoxComponent* BoxComponent;
    EPortalType PortalType;
    AQLPortal* Spouse;
    AQLWeaponPortalGun* PortalOwner;
    TMap<FString, AActor*> Roll;

    //------------------------------------------------------------
    // four preset materials are imported at ctor
    // --- 2 x default material: filled single-color ellipse
    // --- 2 x material: hollow single-color ellipse, to be filled by
    //     the render target at runtime
    //------------------------------------------------------------
    UMaterial* BluePortalMaterial;
    UMaterial* OrangePortalMaterial;
    UMaterial* BluePortalDefaultMaterial;
    UMaterial* OrangePortalDefaultMaterial;

    UTextureRenderTarget2D* PortalRenderTarget;
    UMaterialInstanceDynamic* PortalDynamicMaterial;
    USceneCaptureComponent2D* PortalCamera;
    UBoxComponent* PortalCameraComp;
    UStaticMeshComponent* HelperStaticMeshComponent;
};
