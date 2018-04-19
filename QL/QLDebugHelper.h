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

#include "GameFramework/Actor.h"
#include "QLDebugHelper.generated.h"

//------------------------------------------------------------
// usage:
// add to class declaration
// AQLDebugHelper* DebugHelper;
//
// add to BeginPlay()
// DebugHelper = GetWorld()->SpawnActor<AQLDebugHelper>(AQLDebugHelper::StaticClass());
// DebugHelper->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
// DebugHelper->SetActorRelativeLocation(FVector(200.0f, 0.0f, 0.0f));
//------------------------------------------------------------
UCLASS()
class QL_API AQLDebugHelper : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    AQLDebugHelper();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    UStaticMeshComponent* XAxis;
    UStaticMeshComponent* YAxis;
    UStaticMeshComponent* ZAxis;
    UTextRenderComponent* XAxisLabel;
    UTextRenderComponent* YAxisLabel;
    UTextRenderComponent* ZAxisLabel;
    UBoxComponent* BoxComponent;
};
