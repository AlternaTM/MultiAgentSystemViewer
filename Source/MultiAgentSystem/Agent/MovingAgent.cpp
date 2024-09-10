#include "MovingAgent.h"

AMovingAgent::AMovingAgent()
{
	PrimaryActorTick.bCanEverTick = true;

	bIsReversing = false;

    CurrentIndex = -1;
    PreviousTime = -1.0f;

	MovementTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMeshComponent"));
	ProceduralMeshComponent->SetupAttachment(RootComponent);

	bUsingStaticMesh = true;

	StaticMeshComponent->SetVisibility(false);
	ProceduralMeshComponent->SetVisibility(false);

	static ConstructorHelpers::FObjectFinder<UMaterial> BaseMaterial(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (BaseMaterial.Succeeded())
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial.Object, this);

		StaticMeshComponent->SetMaterial(0, DynamicMaterial);
		ProceduralMeshComponent->SetMaterial(0, DynamicMaterial);
	}
}

void AMovingAgent::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMovingAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMovingAgent::SetupAgentData(
	ExternalMeshController* Controller,
	TArray<FAgentAppearance> AppearanceData,
	UCurveVector* MovementCurve)
{
	MeshController = Controller;

	AgentAppearanceData = AppearanceData;
	FirstTimestamp = AgentAppearanceData[0].Timestamp;

	FOnTimelineVector TimelineCallback;
	TimelineCallback.BindUFunction(this, FName("GetTimelineVector"));

	MovementTimeline->AddInterpVector(MovementCurve, TimelineCallback);
	MovementTimeline->SetPlayRate(1.0f);
	MovementTimeline->SetLooping(false);
}

void AMovingAgent::StartExecution()
{
	bIsReversing = false;

	if (MovementTimeline)
	{
		MovementTimeline->Play();
	}
}

void AMovingAgent::StopExecution()
{
	if (MovementTimeline)
	{
		MovementTimeline->Stop();
	}
}

void AMovingAgent::ReverseExecution()
{
	bIsReversing = true;

	if (MovementTimeline)
	{
		MovementTimeline->Reverse();
	}
}

void AMovingAgent::GetTimelineVector(FVector Value)
{
	SetActorLocation(Value);

	float CurrentTime = MovementTimeline->GetPlaybackPosition();
    UpdateAgentAppearance(CurrentTime);
    CheckAgentVisibility(CurrentTime);
}

void AMovingAgent::CheckAgentVisibility(float CurrentTime)
{
    if (CurrentTime >= FirstTimestamp)
    {
        if (!StaticMeshComponent->IsVisible() && !ProceduralMeshComponent->IsVisible())
        {
            StaticMeshComponent->SetVisibility(bUsingStaticMesh);
            ProceduralMeshComponent->SetVisibility(!bUsingStaticMesh);
        }
    }
    else if (StaticMeshComponent->IsVisible() || ProceduralMeshComponent->IsVisible())
    {
        StaticMeshComponent->SetVisibility(false);
        ProceduralMeshComponent->SetVisibility(false);
    }
}

void AMovingAgent::UpdateAgentAppearance(float CurrentTime)
{
    int32 NextIndex = bIsReversing ? CurrentIndex - 1 : CurrentIndex + 1;

    if (AgentAppearanceData.IsValidIndex(NextIndex))
    {
        float Timestamp = AgentAppearanceData[bIsReversing ? CurrentIndex : NextIndex].Timestamp;

        if ((bIsReversing && CurrentTime < Timestamp) ||
            (!bIsReversing && CurrentTime >= Timestamp))
        {
            CurrentIndex = NextIndex;
            ChangeColor(AgentAppearanceData[CurrentIndex].Color);
            ChangeMesh(AgentAppearanceData[CurrentIndex].Mesh);
            SetActorRotation(AgentAppearanceData[CurrentIndex].Rotation);
        }
    }

    PreviousTime = CurrentTime;
}

void AMovingAgent::ChangeMesh(const FString& MeshName)
{
	if (MeshName.IsEmpty() || MeshName.Equals(CurrentMeshName))
	{
		return;
	}

	if (MeshName.EndsWith(".obj"))
	{
		ChangeToProceduralMesh(MeshName);
	}
	else
	{
		ChangeToStaticMesh(MeshName);
	}
}

void AMovingAgent::ChangeToStaticMesh(const FString& MeshName)
{
	FString CorrectedMeshName = MeshName;
	CorrectedMeshName[0] = FChar::ToUpper(MeshName[0]);

	FString MeshPath = FString::Printf(TEXT("/Engine/BasicShapes/%s.%s"), *CorrectedMeshName, *CorrectedMeshName);
	UStaticMesh* LoadedMesh = LoadObject<UStaticMesh>(this, *MeshPath);

	if (!LoadedMesh)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Error: StaticMesh not found!"));
			return;
		}
	}
	CurrentMeshName = MeshName;

	StaticMeshComponent->SetStaticMesh(LoadedMesh);
	StaticMeshComponent->SetMaterial(0, DynamicMaterial);

	bUsingStaticMesh = true;

	ProceduralMeshComponent->SetVisibility(!bUsingStaticMesh);
	StaticMeshComponent->SetVisibility(bUsingStaticMesh);
}

void AMovingAgent::ChangeToProceduralMesh(const FString& MeshName)
{
	FMeshData* MeshData = MeshController->GetMeshData(MeshName);
	if (!MeshData)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Vertices: %d, Triangles: %d"), MeshData->Vertices.Num(), MeshData->Triangles.Num());

	if (MeshData->Vertices.Num() == 0 || MeshData->Triangles.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid mesh data: vertices or triangles are empty"));
		return;
	}

	CurrentMeshName = MeshName;

	ProceduralMeshComponent->CreateMeshSection(
		0, MeshData->Vertices, MeshData->Triangles,
		MeshData->Normals, MeshData->UVs, TArray<FColor>(),
		TArray<FProcMeshTangent>(), true);

	ProceduralMeshComponent->SetMaterial(0, DynamicMaterial);

	bUsingStaticMesh = false;

	StaticMeshComponent->SetVisibility(bUsingStaticMesh);
	ProceduralMeshComponent->SetVisibility(true);
}

void AMovingAgent::ChangeColor(const FLinearColor& Color)
{
	DynamicMaterial->SetVectorParameterValue("Color", Color);
}