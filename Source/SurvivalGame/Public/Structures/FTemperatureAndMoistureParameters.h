#pragma once
#include "FBiomMapGenerateParameters.h"
#include "FTemperatureAndMoistureParameters.generated.h"

USTRUCT()
struct FTemperatureAndMoistureParameters
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	FBiomMapGenerateParameters TemperatureParameters;
	UPROPERTY(EditAnywhere)
	FBiomMapGenerateParameters MoistureParameters;
};