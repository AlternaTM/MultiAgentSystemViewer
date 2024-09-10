#pragma once

#include "CoreMinimal.h"

#include "Curves/CurveVector.h"
#include "../Agent/AgentAppearance.h"

class MULTIAGENTSYSTEM_API CSVParser
{
public:
	bool ParseCSV(const FString& FilePath);

    UCurveVector* GetAgentPath();

    TArray<FAgentAppearance> GetAgentAppearance();

private:
    TArray<FString> ParseCSVLine(const FString& Line);

    FLinearColor HexToLinearColor(const FString& HexString);

    TMap<FString, int32> Columns;

    UCurveVector* AgentPath;

    TArray<FAgentAppearance> AgentAppearanceData;
};
