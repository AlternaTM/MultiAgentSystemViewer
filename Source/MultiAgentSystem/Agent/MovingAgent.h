#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/TimelineComponent.h"
#include "Curves/CurveVector.h"
#include "ProceduralMeshComponent.h"
#include "../MeshController/ExternalMeshController.h"
#include "AgentAppearance.h"

#include "MovingAgent.generated.h"

UCLASS()
class MULTIAGENTSYSTEM_API AMovingAgent : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovingAgent();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void StartExecution();

	UFUNCTION(BlueprintCallable)
	void StopExecution();

	UFUNCTION(BlueprintCallable)
	void ReverseExecution();

	void SetupAgentData(
		ExternalMeshController* Controller, 
		TArray<FAgentAppearance> AppearanceData, 
		UCurveVector* MovementCurve);

	UFUNCTION()
	void GetTimelineVector(FVector Value);

private:
    void CheckAgentVisibility(float CurrentTime);

    void UpdateAgentAppearance(float CurrentTime);

	void ChangeMesh(const FString& MeshName);

	void ChangeToStaticMesh(const FString& MeshName);

	void ChangeToProceduralMesh(const FString& MeshName);

	void ChangeColor(const FLinearColor& Color);

	bool bIsReversing;
	UTimelineComponent* MovementTimeline;
	float FirstTimestamp;

	ExternalMeshController* MeshController;
	TArray<FAgentAppearance> AgentAppearanceData;
	int32 CurrentIndex;
	float PreviousTime;

	bool bUsingStaticMesh;
	FString CurrentMeshName;
	UStaticMeshComponent* StaticMeshComponent;
	UProceduralMeshComponent* ProceduralMeshComponent;

	UMaterialInstanceDynamic* DynamicMaterial;
};