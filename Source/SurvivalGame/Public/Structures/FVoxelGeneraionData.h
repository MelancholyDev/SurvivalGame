#pragma once
#include "FGenerationParameters.h"
#include "Enums/EBiomType.h"

#include "FVoxelGeneraionData.generated.h"

USTRUCT(BlueprintType)
struct FVoxelGenerationData
{
	GENERATED_USTRUCT_BODY()
	bool IsAddDepth;
	float Threshold3D;
	int Depth;
	int CaveStart;
	float VoxelSize;
	int NoiseScale;
	int ChunkSize;
	int MapSize;
	float NoiseDensity3D;
	float **Map;
	float WaterLevel;
	EBiomType** BiomMap;
	float ***UndergroundMap;
	FVoxelGenerationData(): IsAddDepth(false), Threshold3D(0), Depth(0), CaveStart(0), VoxelSize(0), NoiseScale(0),
	                        ChunkSize(0),
	                        MapSize(0),
	                        NoiseDensity3D(0),
	                        Map(nullptr),
	                        WaterLevel(0),
	                        BiomMap(nullptr),
	                        UndergroundMap(nullptr)
	{
		
	}
	FVoxelGenerationData(FGenerationParameters GenerationParameters,int MapSizeParam,float **MapParam,EBiomType **BiomMapParam,float*** UndergroundMapParam)
	{
		IsAddDepth = GenerationParameters.UndergroundParameters.IsAddDepth;
		Threshold3D = GenerationParameters.UndergroundParameters.Threshold3D;
		Depth = GenerationParameters.UndergroundParameters.Depth;
		CaveStart = GenerationParameters.UndergroundParameters.CaveStart;
		VoxelSize = GenerationParameters.VoxelSize;
		NoiseScale = GenerationParameters.NoiseScale;
		ChunkSize=GenerationParameters.ChunkSize;
		MapSize=MapSizeParam;
		Map=MapParam;
		BiomMap=BiomMapParam;
		UndergroundMap=UndergroundMapParam;
		WaterLevel = GenerationParameters.WaterParameters.WaterLevel;
		NoiseDensity3D = GenerationParameters.UndergroundParameters.NoiseDenisty3D;
	}
};
