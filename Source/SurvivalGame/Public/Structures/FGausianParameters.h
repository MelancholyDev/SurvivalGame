#pragma once
#include "FGausianParameters.generated.h"

USTRUCT()
struct FGausianParameters
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	bool IsApplyGausianFilter;
	UPROPERTY(EditAnywhere)
	int KernelSize;
	UPROPERTY(EditAnywhere)
	float Sigma;
};
