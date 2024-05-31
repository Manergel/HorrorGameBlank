// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ADoorActor::ADoorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the door mesh component
	DoorMesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("DoorMesh"));
	RootComponent = DoorMesh;

	// Create the door collider component
	DoorCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorCollider"));
	DoorCollider->SetupAttachment(DoorMesh);

	// Set the collision box size
	DoorCollider->SetBoxExtent(FVector(50.0f, 10.0f, 100.0f)); //Adjust size as needed
	DoorCollider->SetCollisionProfileName(TEXT("BlockAll"));

	// Initialize variables
	bIsOpen = false;
	OpenAngle = 90.0f; // The angle to open the door

}

// Called when the game starts or when spawned
void ADoorActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoorActor::Interact()
{
	if (bIsOpen)
	{
		CloseDoor();
	}
	else
	{
		OpenDoor();
	}

}

void ADoorActor::OpenDoor()
{
	FRotator NewRotation = FRotator(0.0f, OpenAngle, 0.0f);
	DoorMesh->SetRelativeRotation(NewRotation);
	bIsOpen = true;
}

void ADoorActor::CloseDoor()
{
	FRotator NewRotation = FRotator(0.0f, 0.0f, 0.0f);
	DoorMesh->SetRelativeRotation(NewRotation);
	bIsOpen = false;
}

