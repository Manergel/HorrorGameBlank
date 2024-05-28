// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class HORRORGAMEBLANK_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Player's camera component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* Camera;

	// Movement speeds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RunSpeed = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CrouchSpeed = 300.0f;

	// Crouch settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CrouchHeight = 40.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float DefaultCameraZ = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CrouchCameraZ = 20.0f;

	// Handles moving forward/backward
	void MoveForward(float InputValue);

	// Handles moving right/left
	void MoveRight(float InputValue);

	// Handles turning the camera left/right
	void TurnCamera(float InputValue);

	// Handles looking up/down
	void LookUp(float InputValue);

	// Handles crouching
	void StartCrouch();
	void StopCrouch();

	// Handles running
	void StartRun();
	void StopRun();

private:
	// Stores the default capsule half-height
	float DefaultCapsuleHalfHeight;

	// Checks if there's enough space to stand up
	bool CanStandUp() const;
	
	// Check if player is crouch
	bool bIsCrouching;
};

