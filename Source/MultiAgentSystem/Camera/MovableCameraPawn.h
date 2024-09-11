#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

#include "MovableCameraPawn.generated.h"

UCLASS()
class MULTIAGENTSYSTEM_API AMovableCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	AMovableCameraPawn();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void MoveUp(float Value);
	void Turn(float Value);
	void LookUp(float Value);

private:
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	UFloatingPawnMovement* MovementComponent;
};