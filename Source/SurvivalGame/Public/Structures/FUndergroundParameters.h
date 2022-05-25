#pragma once
#include "FPerlinParameters.h"
#include "FWormSettings.h"
#include "FUndergroundParameters.generated.h"

USTRUCT()
struct FUndergroundParameters
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	float NoiseDenisty3D;
	UPROPERTY(EditAnywhere)
	bool IsAddDepth;
	UPROPERTY(EditAnywhere)
	float Threshold3D;
	UPROPERTY(EditAnywhere)
	int Depth;
	UPROPERTY(EditAnywhere)
	int CaveStart;
	UPROPERTY(EditAnywhere)
	FPerlinParameters UpperLevelParameters;
	UPROPERTY(EditAnywhere)
	FPerlinParameters FirstNoise;
	UPROPERTY(EditAnywhere)
	FPerlinParameters SecondNoise;
	UPROPERTY(EditAnywhere)
	FPerlinParameters WormPlaceNoise;
	UPROPERTY(EditAnywhere)
	FWormSettings WormSettings;
};
