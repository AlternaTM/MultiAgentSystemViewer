#include "AgentSpawnController.h"

#include "../Agent/MovingAgent.h"

AAgentSpawnController::AAgentSpawnController()
{
	PrimaryActorTick.bCanEverTick = false;

	Parser = new CSVParser();
	MeshController = new ExternalMeshController();
}

void AAgentSpawnController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAgentSpawnController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AAgentSpawnController::SpawnNewAgent(const FString& CSVPath)
{
	if (!Parser->ParseCSV(CSVPath))
	{
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("CSV parsing failed!"));
		return false;
	}

	UCurveVector* MovementCurve = Parser->GetAgentPath();
	TArray<FAgentAppearance> AppearanceData = Parser->GetAgentAppearance();

	for (FAgentAppearance Appearance : AppearanceData)
	{
        if (!MeshController->SaveMeshData(Appearance.Mesh))
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Invalid .obj path found!")));
            return false;
        }
	}

    FVector Location = FVector(0.0f, 0.0f, 0.0f);
    FRotator Rotation = FRotator::ZeroRotator;
    AMovingAgent* NewAgent = GetWorld()->SpawnActor<AMovingAgent>(AMovingAgent::StaticClass(), Location, Rotation);

	NewAgent->SetupAgentData(MeshController, AppearanceData, MovementCurve);

    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Agent %s spawned successfully!"), *NewAgent->GetName()));
    return true;
}