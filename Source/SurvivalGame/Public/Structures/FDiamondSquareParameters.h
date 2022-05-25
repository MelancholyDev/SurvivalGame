#pragma once
#include "Enums/EDisplacementType.h"
#include "FDiamondSquareParameters.generated.h"

USTRUCT(BlueprintType)
struct FDiamondSquareParameters
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	float Roughness;
	UPROPERTY(EditAnywhere)
	int MapMultiplier;
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDisplacementType> DisplacementType;
	UPROPERTY(EditAnywhere)
	int Seed;
	
};
