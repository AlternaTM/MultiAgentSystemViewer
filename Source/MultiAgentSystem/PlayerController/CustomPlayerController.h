#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"

UCLASS()
class MULTIAGENTSYSTEM_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void BeginPlay();

	UFUNCTION(BlueprintCallable)
	void SwitchCamera();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<APawn> MovableCameraClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<APawn> TopDownCameraClass;

private:
	bool bIsInTopDownView;

	FRotator SavedCameraRotation;
};