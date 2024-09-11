#include "CSVParser.h"

bool CSVParser::ParseCSV(const FString& FilePath)
{
	FString CSVContent;

    if (!FFileHelper::LoadFileToString(CSVContent, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load CSV file: %s"), *FilePath);
        return false;
    }

    Columns.Empty();
    AgentAppearanceData.Empty();

    TArray<FString> Lines;
    CSVContent.ReplaceInline(TEXT("\r\n"), TEXT("\n"));
    CSVContent.ParseIntoArray(Lines, TEXT("\n"), true);

    if (Lines.Num() < 2)
    {
        UE_LOG(LogTemp, Error, TEXT("CSV file is empty: %s"), *FilePath);
        return false;
    }

    TArray<FString> Headers = ParseCSVLine(Lines[0]);
    for (int32 i = 0; i < Headers.Num(); ++i)
    {
        Columns.Add(Headers[i].ToLower(), i);
    }

    if (!Columns.Contains(TEXT("timestamp")) ||
        !Columns.Contains(TEXT("x")) ||
        !Columns.Contains(TEXT("y")) ||
        !Columns.Contains(TEXT("z")) ||
        !Columns.Contains(TEXT("mesh"))||
        !Columns.Contains(TEXT("color")))
    {
        UE_LOG(LogTemp, Error, TEXT("CSV file does not contain the required columns: %s"), *FilePath);
        return false;
    }

    AgentPath = NewObject<UCurveVector>();

    if (!AgentPath)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create dynamic curve."));
        return false;
    }

    FRichCurve* XCurve = &AgentPath->FloatCurves[0];
    FRichCurve* YCurve = &AgentPath->FloatCurves[1];
    FRichCurve* ZCurve = &AgentPath->FloatCurves[2];

    TArray<float> Timestamps;
    FVector PreviousPosition;
    FAgentAppearance* PreviousAppearance = nullptr;

    for (int32 i = 1; i < Lines.Num(); ++i)
    {
        TArray<FString> ParsedLine = ParseCSVLine(Lines[i]);

        if (ParsedLine.Num() == Columns.Num())
        {
            float Timestamp = FCString::Atof(*ParsedLine[Columns[TEXT("timestamp")]]);

            if (!Timestamps.Contains(Timestamp))
            {
                float X = FCString::Atof(*ParsedLine[Columns[TEXT("x")]]);
                float Y = FCString::Atof(*ParsedLine[Columns[TEXT("y")]]);
                float Z = FCString::Atof(*ParsedLine[Columns[TEXT("z")]]);

                FVector CurrentPosition(X, Y, Z);

                XCurve->AddKey(Timestamp, X);
                YCurve->AddKey(Timestamp, Y);
                ZCurve->AddKey(Timestamp, Z);

                FAgentAppearance Appearance;
                Appearance.Timestamp = Timestamp;

                FString HexColor = ParsedLine[Columns[TEXT("color")]];
                FLinearColor Color = HexToLinearColor(HexColor);
                Appearance.Color = Color;

                FString MeshName = ParsedLine[Columns[TEXT("mesh")]];
                Appearance.Mesh = MeshName;

                if (PreviousAppearance)
                {
                    FVector Direction = (CurrentPosition - PreviousPosition).GetSafeNormal();
                    FRotator Rotator = Direction.Rotation();
                    PreviousAppearance->Rotation = Rotator;
                }

                PreviousPosition = CurrentPosition;

                AgentAppearanceData.Add(Appearance);
                PreviousAppearance = &AgentAppearanceData.Last();

                Timestamps.Add(Timestamp);
            }
        }
    }

    if (AgentAppearanceData.Num() > 0)
    {
        AgentAppearanceData.Last().Rotation = FRotator::ZeroRotator;
    }

    AgentAppearanceData.Sort([](const FAgentAppearance& A, const FAgentAppearance& B)
        {
            return A.Timestamp < B.Timestamp;
        });

    return true;
}

TArray<FString> CSVParser::ParseCSVLine(const FString& Line)
{
    TArray<FString> Result;
    Line.ParseIntoArray(Result, TEXT(","), true);
    return Result;
}

FLinearColor CSVParser::HexToLinearColor(const FString& HexString)
{
    FString CleanHexString = HexString.Replace(TEXT("#"), TEXT(""));

    if (CleanHexString.Len() == 6 || CleanHexString.Len() == 8)
    {
        int32 R, G, B = 255;
        FLinearColor LinearColor;

        R = FParse::HexNumber(*CleanHexString.Left(2));
        G = FParse::HexNumber(*CleanHexString.Mid(2, 2));
        B = FParse::HexNumber(*CleanHexString.Mid(4, 2));

        LinearColor = FLinearColor(R / 255.0f, G / 255.0f, B / 255.0f);
        return LinearColor;
    }

    return FLinearColor::Black;
}

UCurveVector* CSVParser::GetAgentPath()
{
    return AgentPath;
}

TArray<FAgentAppearance> CSVParser::GetAgentAppearance()
{
    return AgentAppearanceData;
}