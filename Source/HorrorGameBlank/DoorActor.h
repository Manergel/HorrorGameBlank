#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorActor.generated.h"

class UBoxComponent;

UCLASS()
class HORRORGAMEBLANK_API ADoorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// The function to call when the door is interacted with
	UFUNCTION() void Interact();

private:
	// Component to represent the door mesh
	UPROPERTY(VisibleAnywhere) UStaticMeshComponent* DoorMesh;

	// Collision component for the door
	UPROPERTY(VisibleAnywhere) UBoxComponent* DoorCollider;

	// Boolean to track the door's state (open/closed)
	bool bIsOpen;

	// The angle the door will open to
	UPROPERTY(EditAnywhere) float OpenAngle;

	// Function to handle opening the door
	void OpenDoor();

	// Function to handle closing the door
	void CloseDoor();

};
