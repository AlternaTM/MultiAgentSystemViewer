#include "CustomPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "../Camera/MovableCameraPawn.h"
#include "../Camera/TopDownCameraPawn.h"

void ACustomPlayerController::BeginPlay()
{
    Super::BeginPlay();

    bShowMouseCursor = true;
    SetInputMode(FInputModeGameAndUI());

    if (WidgetClass)
    {
        UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(this, WidgetClass);
        if (WidgetInstance)
        {
            WidgetInstance->AddToViewport();
        }
    }

    bIsInTopDownView = false;
}

void ACustomPlayerController::SwitchCamera()
{
    AMovableCameraPawn* MovableCamera = Cast<AMovableCameraPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), MovableCameraClass));
    if (bIsInTopDownView)
    {
        if (MovableCamera)
        {
            Possess(MovableCamera);

            SetControlRotation(SavedCameraRotation);
        }
    }
    else
    {
        if (MovableCamera)
        {
            SavedCameraRotation = GetControlRotation();
        }

        ATopDownCameraPawn* TopDownCamera = Cast<ATopDownCameraPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), TopDownCameraClass));
        if (TopDownCamera)
        {
            Possess(TopDownCamera);
        }
    }

    bIsInTopDownView = !bIsInTopDownView;
}