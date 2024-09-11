#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

#include "TopDownCameraPawn.generated.h"

UCLASS()
class MULTIAGENTSYSTEM_API ATopDownCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	ATopDownCameraPawn();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void MoveUp(float Value);
    void MoveRight(float Value);
    void MoveForward(float Value);

private:
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere)
    UFloatingPawnMovement* MovementComponent;
};