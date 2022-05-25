#pragma once
#include "FDiamondSquareParameters.h"
#include "FGausianParameters.h"
#include "FPerlinNoiseParameters.h"
#include "FTemperatureAndMoistureParameters.h"
#include "FUndergroundParameters.h"
#include "FWaterParameters.h"
#include "Enums/EGenerationType.h"
#include "FGenerationParameters.generated.h"

USTRUCT()
struct FGenerationParameters
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EGenerationType> GenerationType;
	UPROPERTY(EditAnywhere)
	bool IsApplyGausianFilter;
	UPROPERTY(EditAnywhere)
	bool IsAddBezierFunction;
	UPROPERTY(EditAnywhere)
	int RenderRange;
	UPROPERTY(EditAnywhere)
	int ChunkSize;
	UPROPERTY(EditAnywhere)
	int VoxelSize;
	UPROPERTY(EditAnywhere)
	float NoiseScale;
	UPROPERTY(EditAnywhere)
	FDiamondSquareParameters DiamondSquareParameters;
	UPROPERTY(EditAnywhere)
	FPerlinNoiseParameters PerlinNoiseParameters;
	UPROPERTY(EditAnywhere)
	FGausianParameters GausianParameters;
	UPROPERTY(EditAnywhere)
	FTemperatureAndMoistureParameters TemperatureAndMoistureParameters;
	UPROPERTY(EditAnywhere)
	FUndergroundParameters UndergroundParameters;
	UPROPERTY(EditAnywhere)
	FWaterParameters WaterParameters;

	FGenerationParameters(): GenerationType(), IsApplyGausianFilter(false), IsAddBezierFunction(false), RenderRange(0),
	                         ChunkSize(0),
	                         VoxelSize(0), NoiseScale(0),
	                         DiamondSquareParameters(),
	                         PerlinNoiseParameters(),
	                         GausianParameters(),
	                         TemperatureAndMoistureParameters(),
	                         UndergroundParameters(),
	                         WaterParameters()
	{
	}

	FGenerationParameters(const TEnumAsByte<EGenerationType>& GenerationType, bool bIsApplyGausianFilter,
	                      bool IsAddBezierFunction, int RenderRange, int ChunkSize, int VoxelSize, float NoiseScale,
	                      const FDiamondSquareParameters& DiamondSquareParameters,
	                      const FPerlinNoiseParameters& PerlinNoiseParameters,
	                      const FGausianParameters& GausianParameters,
	                      const FTemperatureAndMoistureParameters& TemperatureAndMoistureParameters,
	                      const FUndergroundParameters& UndergroundParameters, const FWaterParameters& WaterParameters)
		: GenerationType(GenerationType),
		  IsApplyGausianFilter(bIsApplyGausianFilter),
		  IsAddBezierFunction(IsAddBezierFunction),
		  RenderRange(RenderRange),
		  ChunkSize(ChunkSize),
		  VoxelSize(VoxelSize),
		  NoiseScale(NoiseScale),
		  DiamondSquareParameters(DiamondSquareParameters),
		  PerlinNoiseParameters(PerlinNoiseParameters),
		  GausianParameters(GausianParameters),
		  TemperatureAndMoistureParameters(TemperatureAndMoistureParameters),
		  UndergroundParameters(UndergroundParameters),
		  WaterParameters(WaterParameters)
	{
	}
};
