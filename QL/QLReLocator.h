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
#include "QLReLocator.generated.h"

//------------------------------------------------------------
// Relocate player properly as soon as he overlaps
// with AQLReLocator
//------------------------------------------------------------
UCLASS()
class QL_API AQLReLocator : public AQLActor
{
	GENERATED_BODY()
	
public:
    //------------------------------------------------------------
	// Sets default values for this actor's properties
    //------------------------------------------------------------
	AQLReLocator();

protected:
    //------------------------------------------------------------
	// Called when the game starts or when spawned
    //------------------------------------------------------------
	virtual void BeginPlay() override;

public:
    //------------------------------------------------------------
	// Called every frame
    //------------------------------------------------------------
	virtual void Tick(float DeltaTime) override;

    //------------------------------------------------------------
    // Upon overlapping, teleport player immediately and cancel all
    // movement
    //------------------------------------------------------------
    UFUNCTION(BlueprintCallable)
    void OnOverlapBeginForActor(AActor* OverlappedActor, AActor* OtherActor);
	
};
