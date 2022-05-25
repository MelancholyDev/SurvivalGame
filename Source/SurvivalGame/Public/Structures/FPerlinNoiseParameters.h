#pragma once
#include "FPerlinParameters.h"
#include "Enums/EBiomType.h"
#include "FPerlinNoiseParameters.generated.h"

USTRUCT()
struct FPerlinNoiseParameters
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	FPerlinParameters Smooth;
	UPROPERTY(EditAnywhere)
	FPerlinParameters Sharp;
	UPROPERTY(EditAnywhere)
	int MapSizeMultiplier;
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EBiomType> DefaultBiom;
	UPROPERTY(EditAnywhere)
	bool IsTest;
	UPROPERTY(EditAnywhere)
	bool IsInvert;
};