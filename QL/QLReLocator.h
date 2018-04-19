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

UCLASS()
class QL_API AQLReLocator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQLReLocator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
