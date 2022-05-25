#include "Math/DiamondSquare.h"

DiamondSquare::DiamondSquare(FDiamondSquareParameters Parameters, int Size)
{
	CurrentMap = nullptr;
	Roughness = Parameters.Roughness;
	CurrentRoughness=Roughness;
	DisplacementType = Parameters.DisplacementType;
	MapSize = Size;
	Seed = Parameters.Seed;
}

void DiamondSquare::GenerateMap(float** Map)
{
	CurrentRoughness = Roughness;
	FMath::SRandInit(Seed);
	CurrentMap = Map;
	for (int i = 0; i < MapSize; i++)
		for (int j = 0; j < MapSize; j++)
		{
			Map[i][j] = 0;
		}
	InitializeCorners();
	DiamondAlgorithm(MapSize - 1, 1);
}

void DiamondSquare::InitializeCorners()
{
	int Shift = MapSize - 1;
	for (int i = 0; i < MapSize; i += Shift)
		for (int j = 0; j < MapSize; j += Shift)
			CurrentMap[i][j] = 0;
}

void DiamondSquare::DiamondAlgorithm(int Size, int Iteration)
{
	if (Size < 2)
	{
		return;
	}
	const int Half = Size / 2;
	for (int i = Half; i < MapSize; i += Size)
		for (int j = Half; j < MapSize; j += Size)
		{
			DiamondStep(i, j, Half, Iteration);
		}
	int Column = 0;
	for (int i = 0; i < MapSize; i += Half)
	{
		if (Column % 2 == 0)
			for (int j = Half; j < MapSize; j += Size)
				SquareStep(i, j, Half, Iteration);
		else
			for (int j = 0; j < MapSize; j += Size)
				SquareStep(i, j, Half, Iteration);
		Column++;
	}
	if (DisplacementType == VERSION_1)
		CurrentRoughness = CurrentRoughness / 2;
	DiamondAlgorithm(Half, Iteration + 1);
}

void DiamondSquare::SquareStep(int X, int Y, int Reach, int Iteration)
{
	float Sum = 0;

	Sum += CurrentMap[(X - Reach + MapSize - 1) % (MapSize - 1)][Y];
	Sum += CurrentMap[(X + Reach) % (MapSize - 1)][Y];
	Sum += CurrentMap[X][(Y - Reach + MapSize - 1) % (MapSize - 1)];
	Sum += CurrentMap[X][(Y + Reach) % (MapSize - 1)];

	const float Avg = Sum / 4;
	CurrentMap[X][Y] = CalculateDisplacement(Avg, Iteration);
}

void DiamondSquare::DiamondStep(int X, int Y, int Reach, int Iteration)
{
	float Sum = 0;

	Sum += CurrentMap[X - Reach][Y - Reach];
	Sum += CurrentMap[X - Reach][Y + Reach];
	Sum += CurrentMap[X + Reach][Y - Reach];
	Sum += CurrentMap[X + Reach][Y + Reach];

	const float Avg = Sum / 4;
	CurrentMap[X][Y] = CalculateDisplacement(Avg, Iteration);
}

float DiamondSquare::CalculateDisplacement(float Avg, float Iteration)
{
	switch (DisplacementType)
	{
	case VERSION_1: { return CalculateFinalV1(Avg); }
	case VERSION_2: { return CalculateFinalV2(Avg, Iteration); }
	default: return 0;
	}
}

float DiamondSquare::CalculateFinalV1(float Avg)
{
	const float RandomAdd = RandomValue(0, 1);
	return Avg + (RandomAdd * 2 * CurrentRoughness) - CurrentRoughness;
}

float DiamondSquare::CalculateFinalV2(float Avg, float Iteration)
{
	const float RandomAdd = RandomValue(-FMath::Pow(CurrentRoughness, Iteration),
	                                    FMath::Pow(CurrentRoughness, Iteration));
	return Avg + RandomAdd;
}

float DiamondSquare::RandomValue(float Min, float Max)
{
	const float Diff = Max - Min;
	const float Multiplier = FMath::SRand();
	return Min + Diff * Multiplier;
}
