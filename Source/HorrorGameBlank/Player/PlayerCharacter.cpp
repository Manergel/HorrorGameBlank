#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "HorrorGameBlank/DoorActor.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
    : Camera(CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"))), bIsCrouching(false),  bIsCrouchKeyHeldDown(false)
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
    TargetCapsuleHalfHeight = DefaultCapsuleHalfHeight;
    TargetCameraZ = DefaultCameraZ;

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

    if (bIsCrouching && !bIsCrouchKeyHeldDown && CanStandUp())
    {
        StopCrouch();
    }

    // Interpolate capsule height
    float CurrentCapsuleHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
    float NewCapsuleHalfHeight = FMath::FInterpTo(CurrentCapsuleHalfHeight, TargetCapsuleHalfHeight, DeltaTime, CrouchTransitionSpeed);
    GetCapsuleComponent()->SetCapsuleHalfHeight(NewCapsuleHalfHeight);

    // Interpolate camera position
    FVector CameraLocation = Camera->GetRelativeLocation();
    CameraLocation.Z = FMath::FInterpTo(CameraLocation.Z, TargetCameraZ, DeltaTime, CrouchTransitionSpeed);
    Camera->SetRelativeLocation(CameraLocation);
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

    // Bind the Interact function to the "Interact" input action
    PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::Interact);
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
    if (bIsCrouching)
    {
        return; // Prevent starting crouch if already crouching
    }

    Crouch();
    bIsCrouching = true;
    bIsCrouchKeyHeldDown = true;
    GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;

    // Set target values for smooth transition
    TargetCapsuleHalfHeight = CrouchHeight;
    TargetCameraZ = CrouchCameraZ;

}

// Handles stopping crouch
void APlayerCharacter::StopCrouch()
{
    bIsCrouchKeyHeldDown = false;

    if (CanStandUp())
    {
        UnCrouch();
        bIsCrouching = false;
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

        // Set target values for smooth transition
        TargetCapsuleHalfHeight = DefaultCapsuleHalfHeight;
        TargetCameraZ = DefaultCameraZ;

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

// Checks if the character can stand up
bool APlayerCharacter::CanStandUp() const
{
    FVector CapsuleBaseLocation = GetCapsuleComponent()->GetComponentLocation();
    float CapsuleRadius = GetCapsuleComponent()->GetUnscaledCapsuleRadius();
    float CapsuleHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

    // Move the start location up by the crouch height to avoid detecting the floor
    FVector StartLocation = CapsuleBaseLocation + FVector(0.0f, 0.0f, CapsuleRadius + CrouchHeight);
    float StandUpHeightDifference = DefaultCapsuleHalfHeight - CrouchHeight;
    FVector EndLocation = StartLocation + FVector(0.0f, 0.0f, StandUpHeightDifference);

    FCollisionShape StandingCapsule = FCollisionShape::MakeCapsule(CapsuleRadius, DefaultCapsuleHalfHeight);

    FHitResult HitResult;
    bool bHit = GetWorld()->SweepSingleByChannel(
        HitResult,
        StartLocation,
        EndLocation,
        FQuat::Identity,
        ECC_Visibility,
        StandingCapsule
    );

    return !bHit;
}

void APlayerCharacter::Interact()
{
    FHitResult HitResult = PerfomInteractionTrace();
    if (HitResult.GetActor())
    {
        ADoorActor* Door = Cast<ADoorActor>(HitResult.GetActor());
        if (Door)
        {
            Door->Interact();
        }
    }
}

FHitResult APlayerCharacter::PerfomInteractionTrace()
{
    FVector Start = Camera->GetComponentLocation();
    FVector End = Start + (Camera->GetForwardVector() * 200.0f);

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

    return HitResult;
}