#include "TopDownCameraPawn.h"

#include "Components/InputComponent.h"

ATopDownCameraPawn::ATopDownCameraPawn()
{
	PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComponent->SetupAttachment(RootComponent);
    SpringArmComponent->TargetArmLength = 0.0;
    SpringArmComponent->bEnableCameraLag = false;
    SpringArmComponent->bUsePawnControlRotation = false;

    TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
    TopDownCameraComponent->SetupAttachment(SpringArmComponent);
    TopDownCameraComponent->SetProjectionMode(ECameraProjectionMode::Orthographic);
    TopDownCameraComponent->OrthoWidth = 2048.0f;
    TopDownCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 1000.0f));
    TopDownCameraComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

    MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
    MovementComponent->MaxSpeed = 450.0f;
}

void ATopDownCameraPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ATopDownCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATopDownCameraPawn::MoveUp(float Value)
{
    if (Value != 0.0f)
    {
        AddMovementInput(GetActorForwardVector(), Value);
    }
}

void ATopDownCameraPawn::MoveRight(float Value)
{
    if (Value != 0.0f)
    {
        AddMovementInput(GetActorRightVector(), Value);
    }
}

void ATopDownCameraPawn::MoveForward(float Value)
{
    if (Value != 0.0f)
    {
        TopDownCameraComponent->OrthoWidth = FMath::Clamp(TopDownCameraComponent->OrthoWidth - Value * 50.0f, 500.0f, 5000.0f);
    }
}

void ATopDownCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ATopDownCameraPawn::MoveUp);
    PlayerInputComponent->BindAxis("MoveRight", this, &ATopDownCameraPawn::MoveRight);
    PlayerInputComponent->BindAxis("MoveUp", this, &ATopDownCameraPawn::MoveForward);
}