#include "MovableCameraPawn.h"

#include "Components/InputComponent.h"

AMovableCameraPawn::AMovableCameraPawn()
{
	PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComponent->SetupAttachment(RootComponent);
    SpringArmComponent->TargetArmLength = 0.0;
    SpringArmComponent->bEnableCameraLag = true;
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->bDoCollisionTest = false;
    SpringArmComponent->CameraLagSpeed = 3.f;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(SpringArmComponent);

    MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
    MovementComponent->MaxSpeed = 700.0f;
}

void AMovableCameraPawn::BeginPlay()
{
	Super::BeginPlay();

}

void AMovableCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMovableCameraPawn::MoveForward(float Value)
{
    if (Value != 0.0f)
    {
        FRotator ControlRotation = Controller->GetControlRotation();
        FRotator YawRotation(0, ControlRotation.Yaw, 0);
        FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

        AddMovementInput(ForwardDirection, Value);
    }
}

void AMovableCameraPawn::MoveRight(float Value)
{
    if (Value != 0.0f)
    {
        FRotator ControlRotation = Controller->GetControlRotation();
        FRotator YawRotation(0, ControlRotation.Yaw, 0);
        FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(RightDirection, Value);
    }
}

void AMovableCameraPawn::MoveUp(float Value)
{
    if (Value != 0.0f)
    {
        AddMovementInput(GetActorUpVector(), Value);
    }
}

void AMovableCameraPawn::Turn(float Value)
{
    AddControllerYawInput(Value);
}

void AMovableCameraPawn::LookUp(float Value)
{
    AddControllerPitchInput(Value);
}

void AMovableCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &AMovableCameraPawn::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AMovableCameraPawn::MoveRight);
    PlayerInputComponent->BindAxis("MoveUp", this, &AMovableCameraPawn::MoveUp);

    PlayerInputComponent->BindAxis("Turn", this, &AMovableCameraPawn::Turn);
    PlayerInputComponent->BindAxis("LookUp", this, &AMovableCameraPawn::LookUp);
}