#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SpotLightComponent.h"
#include "HeldFlashlight.generated.h"

UCLASS()
class HORRORGAMEBLANK_API AHeldFlashlight : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHeldFlashlight();

protected:
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* FlashlightMesh;

	UPROPERTY(EditDefaultsOnly)
	class USpotLightComponent* SpotlightComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
