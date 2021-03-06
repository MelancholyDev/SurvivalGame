#pragma once
#include "Enums/EBiomType.h"
#include "Engine/DataTable.h"
#include "FbiomData.generated.h"

USTRUCT(BlueprintType)
struct FBiomData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EBiomType> Type;
	UPROPERTY(EditAnywhere)
	int Max = 0;
	UPROPERTY(EditAnywhere)
	int Min = 1;
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

	void CheckValue(int Number)
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