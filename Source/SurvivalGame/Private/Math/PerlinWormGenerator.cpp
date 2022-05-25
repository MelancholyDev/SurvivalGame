#include "Math/PerlinWormGenerator.h"
#include <limits>
#include "Math/PerlinWorm.h"

PerlinWormGenerator::PerlinWormGenerator(int SizeParam, int DepthParam, FWormSettings WormSettingsParam)
{
	InitializeDirectionList();
	WormSettings = WormSettingsParam;
	Depth = DepthParam;
	Size = SizeParam;
}

void PerlinWormGenerator::InitializeDirectionList()
{
	Directions = new TArray<FIntVector>();
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			for (int k = -1; k <= 1; k++)
			{
				if ((i != 0) | (j != 0) | (k != 0))
				{
					Directions->Add(FIntVector(i, j, k));
				}
			}
}

bool PerlinWormGenerator::CheckNeighbours(int X, int Y, int Z, bool IsMaxima)
{
	float Noise = CavePositions[X][Y][Z];
	for (int i = 0; i < Directions->Num(); i++)
	{
		FIntVector Direction = (*Directions)[i];
		int X_Check = X + Direction.X;
		int Y_Check = Y + Direction.Y;
		int Z_Check = Z + Direction.Z;
		if ((X_Check > 0) & (Y_Check > 0) & (Z_Check > 0) & (X_Check < Size) & (Y_Check < Size) & (Z_Check < Depth))
		{
			float NeigbourNoise = CavePositions[X_Check][Y_Check][Z_Check];
			if (FailCondition(Noise, NeigbourNoise, IsMaxima))
			{
				return false;
			}
		}
	}
	return true;
}

FVector PerlinWormGenerator::GetPerlinWormDirection()
{
	return FVector();
}

void PerlinWormGenerator::FindLocalMaximas()
{
	Maximas = new TArray<FIntVector>();
	for (int X = 0; X < Size; X++)
	{
		for (int Y = 0; Y < Size; Y++)
		{
			for (int Z = 0; Z < Depth; Z++)
			{
				 if (CheckNeighbours(X, Y, Z, true))
				 {
				 	Maximas->Add(FIntVector(X, Y, Z));
				 }
			}
		}
	}
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%d"),Maximas->Num()));
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%d"),Size*Size*Depth));
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%d"),Directions->Num()));
}

void PerlinWormGenerator::FindLocalMinimas()
{
	Minimas = new TArray<FIntVector>();
	for (int X = 0; X < Size; X++)
	{
		for (int Y = 0; Y < Size; Y++)
		{
			for (int Z = 0; Z < Depth; Z++)
			{
				if (CheckNeighbours(X, Y, Z, false))
				{
					Minimas->Add(FIntVector(X, Y, Z));
				}
			}
		}
	}
}

bool PerlinWormGenerator::FailCondition(float Noise, float NeighbourNoise, bool IsMaxima)
{
	if (IsMaxima)
	{
		return NeighbourNoise > Noise;
	}
	else
	{
		return NeighbourNoise < Noise;
	}
}

void PerlinWormGenerator::CreateWorm(FIntVector Maxim)
{
	PerlinWorm* Worm;
	if(WormSettings.IsConvergance)
	{
		Worm = new PerlinWorm(UndergroundMap,FirstNoise,SecondNoise,WormSettings, Maxim,FindNearestMinimum(Maxim),Size,Depth);
	}else
	{
		Worm = new PerlinWorm(UndergroundMap,FirstNoise,SecondNoise,WormSettings, Maxim,Size,Depth);
	}
	Worm->MoveLength(WormSettings.WormLength);
}

FIntVector PerlinWormGenerator::FindNearestMinimum(FIntVector CheckPoint)
{
	float Distance=std::numeric_limits<float>::max();
	FVector CheckPointFloat=FVector(CheckPoint);
	FIntVector TargetVector = FIntVector(0,0,0);
	for (int i = 0; i < Minimas->Num(); i++)
	{
		FIntVector Minim = (*Minimas)[i];
		FVector MinimFloat=FVector(Minim);
		float DistanceCheck=FVector::Distance(CheckPointFloat,MinimFloat);
		if(DistanceCheck<Distance)
		{
			Distance=DistanceCheck;
			TargetVector=Minim;
		}
	}
	return TargetVector;
}

void PerlinWormGenerator::GenerateCaves(float*** UndergroundMapParam,float*** FirstNoiseParam, float*** SecondNoiseParam,float*** CavePositionsParam)
{
	UndergroundMap = UndergroundMapParam;
	FirstNoise=FirstNoiseParam;
	SecondNoise=SecondNoiseParam;
	CavePositions =CavePositionsParam;
	for (int i = 0; i < Size; i++)
		for (int j = 0; j < Size; j++)
			for (int k = 0; k < Depth; k++)
			UndergroundMap[i][j][k] = 1;
	
	FindLocalMaximas();
	FindLocalMinimas();
	FMath::SRandInit(355);
	 for (int i = 0; i < Maximas->Num(); i++)
	 {
	 	FIntVector Maxim = (*Maximas)[i];
	 	CreateWorm(Maxim);
	 }
}
