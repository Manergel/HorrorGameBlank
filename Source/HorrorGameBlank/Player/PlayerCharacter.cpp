#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
    : Camera(CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"))), bIsCrouching(false)
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Attach camera to the root component
    Camera->SetupAttachment(RootComponent);
    Camera->bUsePawnControlRotation = true;

    // Set default movement speeds
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

    // Initialize default values
    DefaultCapsuleHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
    DefaultCameraZ = Camera->GetRelativeLocation().Z;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Bind jump action
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

    // Bind crouch actions
    PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::StartCrouch);
    PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APlayerCharacter::StopCrouch);

    // Bind run actions
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APlayerCharacter::StartRun);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &APlayerCharacter::StopRun);

    // Bind movement axes
    PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

    // Bind camera axes
    PlayerInputComponent->BindAxis("TurnCamera", this, &APlayerCharacter::TurnCamera);
    PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUp);
}

// Handles moving forward/backward
void APlayerCharacter::MoveForward(float InputValue)
{
    if (InputValue != 0.0f)
    {
        FVector ForwardDirection = GetActorForwardVector();
        AddMovementInput(ForwardDirection, InputValue);
    }
}

// Handles moving right/left
void APlayerCharacter::MoveRight(float InputValue)
{
    if (InputValue != 0.0f)
    {
        FVector RightDirection = GetActorRightVector();
        AddMovementInput(RightDirection, InputValue);
    }
}

// Handles turning the camera left/right
void APlayerCharacter::TurnCamera(float InputValue)
{
    if (InputValue != 0.0f)
    {
        AddControllerYawInput(InputValue);
    }
}

// Handles looking up/down
void APlayerCharacter::LookUp(float InputValue)
{
    if (InputValue != 0.0f)
    {
        AddControllerPitchInput(InputValue);
    }
}

// Handles starting crouch
void APlayerCharacter::StartCrouch()
{
    Crouch();
    bIsCrouching = true;
    GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;

    // Adjust capsule half-height
    GetCapsuleComponent()->SetCapsuleHalfHeight(CrouchHeight);

    // Log and visualize capsule half-height
    UE_LOG(LogTemp, Warning, TEXT("Crouching: Capsule Half Height = %f"), CrouchHeight);
    DrawDebugCapsule(GetWorld(), GetCapsuleComponent()->GetComponentLocation(), CrouchHeight, GetCapsuleComponent()->GetUnscaledCapsuleRadius(), FQuat::Identity, FColor::Blue, false, 2.0f);

    // Adjust camera height
    FVector CameraLocation = Camera->GetRelativeLocation();
    CameraLocation.Z = CrouchCameraZ;
    Camera->SetRelativeLocation(CameraLocation);

    // Log and visualize camera location
    UE_LOG(LogTemp, Warning, TEXT("Crouching: Camera Z = %f"), CameraLocation.Z);
    DrawDebugSphere(GetWorld(), Camera->GetComponentLocation(), 10.0f, 12, FColor::Blue, false, 2.0f);
}

// Handles stopping crouch
void APlayerCharacter::StopCrouch()
{
    if (CanStandUp())
    {
        UnCrouch();
        bIsCrouching = false;
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

        // Reset capsule half-height
        GetCapsuleComponent()->SetCapsuleHalfHeight(DefaultCapsuleHalfHeight);

        // Log and visualize capsule half-height
        UE_LOG(LogTemp, Warning, TEXT("Standing: Capsule Half Height = %f"), DefaultCapsuleHalfHeight);
        DrawDebugCapsule(GetWorld(), GetCapsuleComponent()->GetComponentLocation(), DefaultCapsuleHalfHeight, GetCapsuleComponent()->GetUnscaledCapsuleRadius(), FQuat::Identity, FColor::Green, false, 2.0f);

        // Reset camera height
        FVector CameraLocation = Camera->GetRelativeLocation();
        CameraLocation.Z = DefaultCameraZ;
        Camera->SetRelativeLocation(CameraLocation);

        // Log and visualize camera location
        UE_LOG(LogTemp, Warning, TEXT("Standing: Camera Z = %f"), CameraLocation.Z);
        DrawDebugSphere(GetWorld(), Camera->GetComponentLocation(), 10.0f, 12, FColor::Green, false, 2.0f);
    }
}

// Handles starting run
void APlayerCharacter::StartRun()
{
    if (!bIsCrouching)
    {
        GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
    }
}

// Handles stopping run
void APlayerCharacter::StopRun()
{
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

bool APlayerCharacter::CanStandUp() const
{
    // Get the character's current location and capsule component
    FVector StartLocation = GetCapsuleComponent()->GetComponentLocation();
    float StandUpHeightDifference = DefaultCapsuleHalfHeight - CrouchHeight;
    FVector EndLocation = StartLocation + FVector(0.0f, 0.0f, StandUpHeightDifference);

    FCollisionShape StandingCapsule = FCollisionShape::MakeCapsule(GetCapsuleComponent()->GetUnscaledCapsuleRadius(), DefaultCapsuleHalfHeight);

    FHitResult HitResult;
    bool bHit = GetWorld()->SweepSingleByChannel(
        HitResult,
        StartLocation,
        StartLocation + FVector(0.0f, 0.0f, StandUpHeightDifference),
        FQuat::Identity,
        ECollisionChannel::ECC_Visibility,
        StandingCapsule
    );

    // Draw the debug capsule
    DrawDebugCapsule(
        GetWorld(),
        StartLocation + FVector(0.0f, 0.0f, StandUpHeightDifference / 2),
        DefaultCapsuleHalfHeight,
        GetCapsuleComponent()->GetUnscaledCapsuleRadius(),
        FQuat::Identity,
        bHit ? FColor::Red : FColor::Green,
        false,
        2.0f,
        0,
        1.0f
    );

    // Log the result
    UE_LOG(LogTemp, Warning, TEXT("CanStandUp: %s"), bHit ? TEXT("False") : TEXT("True"));

    return !bHit;
}


