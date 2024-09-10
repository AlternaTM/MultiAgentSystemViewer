#include "ExternalMeshController.h"

ExternalMeshController::ExternalMeshController()
{
	Parser = new OBJParser();

	OBJMeshDatas = TMap<FString, FMeshData>();
}

bool ExternalMeshController::SaveMeshData(const FString& MeshName)
{
	if (MeshName.EndsWith(".obj") && !OBJMeshDatas.Contains(MeshName))
	{
        if (Parser->ParseOBJ(MeshName))
        {
            FMeshData MeshData = Parser->GetMeshData();
            OBJMeshDatas.Add(MeshName, MeshData);
        }
        else
        {
            return false;
        }
	}
    return true;
}

FMeshData* ExternalMeshController::GetMeshData(const FString& MeshName)
{
    FMeshData* FoundMeshData = OBJMeshDatas.Find(MeshName);

    if (FoundMeshData)
    {
        UE_LOG(LogTemp, Log, TEXT("Mesh data found for: %s"), *MeshName);
        return FoundMeshData;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Mesh data not found for: %s"), *MeshName);
        return nullptr;
    }
}