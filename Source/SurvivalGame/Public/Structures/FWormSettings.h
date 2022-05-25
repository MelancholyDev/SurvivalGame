#pragma once
#include "FWormSettings.generated.h"

USTRUCT()
struct FWormSettings
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	int WormLength;
	UPROPERTY(EditAnywhere)
	int WormWidth;
	UPROPERTY(EditAnywhere)
	float Weight;
	UPROPERTY(EditAnywhere)
	bool IsConvergance;
};
