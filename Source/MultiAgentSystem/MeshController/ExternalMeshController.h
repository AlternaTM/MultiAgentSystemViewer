#pragma once

#include "CoreMinimal.h"

#include "../Parsing/OBJParser.h"

class MULTIAGENTSYSTEM_API ExternalMeshController
{
public:
	ExternalMeshController();

	bool SaveMeshData(const FString& MeshName);

	FMeshData* GetMeshData(const FString& MeshName);

private:
	OBJParser* Parser;

	TMap<FString, FMeshData> OBJMeshDatas;
};
