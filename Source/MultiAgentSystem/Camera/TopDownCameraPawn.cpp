#include "TopDownCameraPawn.h"

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
}

void ATopDownCameraPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ATopDownCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}