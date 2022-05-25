#pragma once
#include "Enums/EDisplacementType.h"
#include "Structures/FGenerationParameters.h"

class DiamondSquare
{
public:
	DiamondSquare(FDiamondSquareParameters Parameters,int Size);
	void GenerateMap(float** Map);
private:
	void DiamondAlgorithm(int Size,int Iteration);
	void InitializeCorners();
	void SquareStep(int X,int Y,int Reach,int Iteration);
	void DiamondStep(int X,int Y,int Reach,int Iteration);
	float CalculateDisplacement(float Avg,float Iteration);
	float CalculateFinalV1(float Avg);
	float CalculateFinalV2(float Avg,float Iteration);
	
	float RandomValue(float Min,float Max);

	float CurrentRoughness;
	float Roughness;
	int MapSize;
	TEnumAsByte<EDisplacementType> DisplacementType;
	float** CurrentMap;
	int Seed;
};
