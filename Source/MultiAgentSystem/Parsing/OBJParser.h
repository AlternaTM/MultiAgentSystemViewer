#pragma once

#include "CoreMinimal.h"

struct FMeshData
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
};

class MULTIAGENTSYSTEM_API OBJParser
{
public:
	bool ParseOBJ(const FString& OBJFile);

    FMeshData GetMeshData()
    {
        return MeshData;
    }

private:
    void ParseVertexLine(const FString& Line);
    void ParseTextureCoordLine(const FString& Line);
    void ParseNormalLine(const FString& Line);
    void ParseFaceLine(const FString& Line);

    FMeshData MeshData;
};
