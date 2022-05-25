#pragma once
#include "Enums/EBiomType.h"
#include "Engine/DataTable.h"
#include "FBiomData.generated.h"

USTRUCT(BlueprintType)
struct FBiomData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EBiomType> Type;
	UPROPERTY(EditAnywhere)
	float Max;
	UPROPERTY(EditAnywhere)
	float Min;
	UPROPERTY(EditAnywhere)
	float B;
	UPROPERTY(EditAnywhere)
	float A;
	UPROPERTY(EditAnywhere)
	float X1;
	UPROPERTY(EditAnywhere)
	float Y1;
	UPROPERTY(EditAnywhere)
	float X2;
	UPROPERTY(EditAnywhere)
	float Y2;

	void CheckValue(float Number)
	{
		if (Number < Min)
		{
			Min = Number;
		}
		if (Number > Max)
		{
			Max = Number;
		}
	}
};