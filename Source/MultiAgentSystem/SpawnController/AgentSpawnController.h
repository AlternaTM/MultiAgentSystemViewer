#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../Parsing/CSVParser.h"
#include "../MeshController/ExternalMeshController.h"

#include "AgentSpawnController.generated.h"

UCLASS()
class MULTIAGENTSYSTEM_API AAgentSpawnController : public AActor
{
	GENERATED_BODY()
	
public:	
	AAgentSpawnController();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
    bool SpawnNewAgent(const FString& CSVFolderPath);

	CSVParser* Parser;

	ExternalMeshController* MeshController;
};