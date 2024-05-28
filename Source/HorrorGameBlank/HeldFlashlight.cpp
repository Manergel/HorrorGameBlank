#include "HorrorGameBlank/HeldFlashlight.h"
#include "Components/SpotLightComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AHeldFlashlight::AHeldFlashlight()
{
	PrimaryActorTick.bCanEverTick = false;

	FlashlightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlashlightMeshComponent"));
	RootComponent = FlashlightMesh;

	SpotlightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotlightComponent"));
	SpotlightComponent->SetupAttachment(FlashlightMesh);
}

// Called when the game starts or when spawned
void AHeldFlashlight::BeginPlay()
{
	Super::BeginPlay();

}

