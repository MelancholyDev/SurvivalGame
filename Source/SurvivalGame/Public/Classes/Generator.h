#pragma once
#include "Math/BezierComputations.h"
#include "Math/DiamondSquare.h"
#include "Math/GausianFilter.h"
#include "Math/PerlinWormGenerator.h"
#include "Structures/FPerlinNoiseParameters.h"
#include "Structures/FTemperatureAndMoistureParameters.h"
#include "Structures/FUndergroundParameters.h"
#include "Structures/FVoxelGeneraionData.h"
#include "Structures/FWormSettings.h"

class Generator
{
public:
	Generator(FGenerationParameters Parameters, UDataTable* Table);
	void GenerateHeightMap(float** Map, EBiomType** BiomMap);
	void GenerateBiomMaps(EBiomType** BiomMap);
	void GenerateSeaMap(float** Map);
	void GenerateCaveMap(float*** UndergroundMap);
private:
	void InitializeBiomData();
	void GenerateWithPerlinNoise(float** Map, EBiomType** BiomMap);
	void GenerateWithDiamondSquare(float** Map);
	float Clamp(float x, float left, float right);
	uint8 GetBiom(float Heat, float Moisture);
	void InvertMap(float** MapForInvert, EBiomType** BiomMap);

	FUndergroundParameters UndergroundParameters;
	FGenerationParameters GenerationParameters;
	FWormSettings WormSettings;
	BezierComputations* BezierComputationsInstance;
	DiamondSquare* DiamondSquareInstance;
	GausianFilter* GausianFilterInstance;
	PerlinWormGenerator* WormGenerator;

	FGausianParameters GausianParameters;
	FPerlinNoiseParameters PerlinNoiseParameters;

	FBiomMapGenerateParameters MoistureParameters;
	FBiomMapGenerateParameters TemperatureParameters;

	FDiamondSquareParameters DiamondSquareParameters;

	TMap<EBiomType, FBiomData> BiomDataSet;
	UDataTable* DataTableBiome;

	int LeftBorder;
	int RightBorder;
	int MapSize;
	int StartCaveIndex;
};
