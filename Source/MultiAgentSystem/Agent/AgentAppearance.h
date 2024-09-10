#pragma once

#include "CoreMinimal.h"
#include "AgentAppearance.generated.h"

USTRUCT(BlueprintType)
struct FAgentAppearance
{
    GENERATED_BODY()

    int32 Timestamp;
    FLinearColor Color;
    FString Mesh;
    FRotator Rotation;
};