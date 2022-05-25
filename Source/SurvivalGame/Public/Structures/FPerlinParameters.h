#pragma once
#include "FPerlinParameters.generated.h"

USTRUCT()
struct FPerlinParameters
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	int Seed;
	UPROPERTY(EditAnywhere)
	int Octaves;
	UPROPERTY(EditAnywhere)
	float Lacunarity;
	UPROPERTY(EditAnywhere)
	float Persistence;
	UPROPERTY(EditAnywhere)
	float NoiseDensity;
	UPROPERTY(EditAnywhere)
	bool ZeroToOne;
};
