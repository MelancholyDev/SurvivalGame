#pragma once
#include "FWaterParameters.generated.h"

USTRUCT()
struct FWaterParameters
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	float WaterLevel;
	UPROPERTY(EditAnywhere)
	bool IsAddRivers;
	UPROPERTY(EditAnywhere)
	int RiverWidth;
};
