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

    // Function to handle interaction
    void Interact();

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

    // Checks if there's enough space to stand up
    bool CanStandUp() const;

private:
    // Stores the default capsule half-height
    float DefaultCapsuleHalfHeight;

    // Check if player is crouching
    bool bIsCrouching;

    // Handles continuous crouch check
    bool bIsCrouchKeyHeldDown;

    // Transition speed for smooth crouching/standing
    float CrouchTransitionSpeed = 10.0f; // Lower value for slower transition

    // Target values for capsule height and camera Z position
    float TargetCapsuleHalfHeight;
    float TargetCameraZ;

    // Trace for interactable objects
    FHitResult PerfomInteractionTrace();
};


