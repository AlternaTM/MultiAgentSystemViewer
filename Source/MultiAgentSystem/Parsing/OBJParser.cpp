#include "OBJParser.h"

#include "ProceduralMeshComponent.h"

bool OBJParser::ParseOBJ(const FString& OBJPath)
{
    FString OBJContent;
    if (!FFileHelper::LoadFileToString(OBJContent, *OBJPath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load OBJ file: %s"), *OBJPath);
        return false;
    }

    MeshData.Vertices.Empty();
    MeshData.UVs.Empty();
    MeshData.Normals.Empty();
    MeshData.Triangles.Empty();

    TArray<FString> Lines;
    OBJContent.ReplaceInline(TEXT("\r\n"), TEXT("\n"));
    OBJContent.ParseIntoArray(Lines, TEXT("\n"), true);

    OBJContent.ParseIntoArrayLines(Lines);

    for (const FString& Line : Lines)
    {
        if (Line.StartsWith(TEXT("v ")))
        {
            ParseVertexLine(Line);
        }
        else if (Line.StartsWith(TEXT("vt ")))
        {
            ParseTextureCoordLine(Line);
        }
        else if (Line.StartsWith(TEXT("vn ")))
        {
            ParseNormalLine(Line);
        }
        else if (Line.StartsWith(TEXT("f ")))
        {
            ParseFaceLine(Line);
        }
    }

    return true;
}

void OBJParser::ParseVertexLine(const FString& Line)
{
    TArray<FString> Tokens;
    Line.ParseIntoArrayWS(Tokens);

    if (Tokens.Num() >= 4)
    {
        float ScaleFactor = 75.0f;
        float X = FCString::Atof(*Tokens[1]) * ScaleFactor;
        float Y = FCString::Atof(*Tokens[2]) * ScaleFactor;
        float Z = FCString::Atof(*Tokens[3]) * ScaleFactor;

        MeshData.Vertices.Add(FVector(X, Z, Y));
    }
}

void OBJParser::ParseTextureCoordLine(const FString& Line)
{
    TArray<FString> Tokens;
    Line.ParseIntoArrayWS(Tokens);

    if (Tokens.Num() >= 3)
    {
        float U = FCString::Atof(*Tokens[1]);
        float V = FCString::Atof(*Tokens[2]);

        MeshData.UVs.Add(FVector2D(U, V));
    }
}

void OBJParser::ParseNormalLine(const FString& Line)
{
    TArray<FString> Tokens;
    Line.ParseIntoArrayWS(Tokens);

    if (Tokens.Num() >= 4)
    {
        float NX = FCString::Atof(*Tokens[1]);
        float NY = FCString::Atof(*Tokens[2]);
        float NZ = FCString::Atof(*Tokens[3]);

        MeshData.Normals.Add(FVector(NX, NZ, NY));
    }
}

void OBJParser::ParseFaceLine(const FString& Line)
{
    TArray<FString> Tokens;
    Line.ParseIntoArrayWS(Tokens);

    if (Tokens.Num() < 4)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid face line: %s"), *Line);
        return;
    }

    TArray<int32> VertexIndices;

    for (int32 i = 1; i < Tokens.Num(); i++)
    {
        TArray<FString> FaceTokens;
        Tokens[i].ParseIntoArray(FaceTokens, TEXT("/"));

        if (FaceTokens.Num() >= 1)
        {
            int32 VertexIndex = FCString::Atoi(*FaceTokens[0]) - 1;
            VertexIndices.Add(VertexIndex);
        }
    }

    for (int32 i = 1; i < VertexIndices.Num() - 1; i++)
    {
        MeshData.Triangles.Add(VertexIndices[0]);
        MeshData.Triangles.Add(VertexIndices[i]);
        MeshData.Triangles.Add(VertexIndices[i + 1]);
    }
}