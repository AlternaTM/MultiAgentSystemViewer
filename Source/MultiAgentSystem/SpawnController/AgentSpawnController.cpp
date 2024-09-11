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

bool AAgentSpawnController::SpawnNewAgent(const FString& CSVFolderPath)
{
    TArray<FString> CSVFiles;
    IFileManager& FileManager = IFileManager::Get();

    FString FullCSVFolderPath = CSVFolderPath;
    FullCSVFolderPath  = FullCSVFolderPath.TrimQuotes();
    if (!FullCSVFolderPath.EndsWith("/"))
    {
        FullCSVFolderPath += "/";
    }

    FileManager.FindFiles(CSVFiles, *FullCSVFolderPath, TEXT("*.csv"));
    bool bOneAgentSpawned = false;

    if (CSVFiles.Num() == 0)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No CSV files found in the folder!"));
        return bOneAgentSpawned;
    }

    for (const FString& CSVFileName : CSVFiles)
    {
        FString FullCSVFilePath = FullCSVFolderPath + CSVFileName;

        if (!Parser->ParseCSV(FullCSVFilePath))
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("CSV parsing failed!"));
            continue;
        }

        UCurveVector* MovementCurve = Parser->GetAgentPath();
        TArray<FAgentAppearance> AppearanceData = Parser->GetAgentAppearance();

        for (FAgentAppearance Appearance : AppearanceData)
        {
            if (!MeshController->SaveMeshData(Appearance.Mesh))
            {
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Invalid .obj path found, changed into default Cube mesh")));
                Appearance.Mesh = "Cube";
            }
        }

        FVector Location = FVector(0.0f, 0.0f, 0.0f);
        FRotator Rotation = FRotator::ZeroRotator;
        AMovingAgent* NewAgent = GetWorld()->SpawnActor<AMovingAgent>(AMovingAgent::StaticClass(), Location, Rotation);

        NewAgent->SetupAgentData(MeshController, AppearanceData, MovementCurve);
        if (!bOneAgentSpawned)
        {
            bOneAgentSpawned = true;
        }
    }
    return bOneAgentSpawned;
}