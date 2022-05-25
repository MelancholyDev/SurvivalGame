#pragma once

#include "CoreMinimal.h"
#include "VoxelChank.h"
#include "Classes/Generator.h"
#include "GameFramework/PlayerController.h"
#include "Factories/CompositeDataTableFactory.h"
#include "Structures/FBiomData.h"
#include "Structures/FVoxelLine.h"
#include "Structures/FVoxelGeneraionData.h"
#include "Structures/FGenerationParameters.h"

#include "GenerationPlayerController.generated.h"

UCLASS()
class SURVIVALGAME_API AGenerationPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	//SerializedParameters
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EGenerationType> GenerationType;
	UPROPERTY(EditAnywhere)
	bool IsApplyGausianFilter;
	UPROPERTY(EditAnywhere)
	bool IsAddBezierFunction;
	UPROPERTY(EditAnywhere)
	int RenderRange;
	UPROPERTY(EditAnywhere)
	int ChunkSize;
	UPROPERTY(EditAnywhere)
	int VoxelSize;
	UPROPERTY(EditAnywhere)
	float NoiseScale;
	UPROPERTY(EditAnywhere)
	FDiamondSquareParameters DiamondSquareParameters;
	UPROPERTY(EditAnywhere)
	FPerlinNoiseParameters PerlinNoiseParameters;
	UPROPERTY(EditAnywhere)
	FGausianParameters GausianParameters;
	UPROPERTY(EditAnywhere)
	FTemperatureAndMoistureParameters TemperatureAndMoistureParameters;
	UPROPERTY(EditAnywhere)
	FUndergroundParameters UndergroundParameters;
	UPROPERTY(EditAnywhere)
	FWaterParameters WaterParameters;
	UPROPERTY(EditAnywhere)
	bool UpdateMap;
	UPROPERTY(EditAnywhere)
	UDataTable* BiomDataSet;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AVoxelChank> ToSpawn;
	//End Serializedparameters
private:
	FGenerationParameters GenerationParameters;
	int MapSize;
	int ChunkLength;
	FIntVector OldCoordinates;
	TArray<FVoxelLine>* Map;
	FActorSpawnParameters* ChunkRenderLines;
	Generator* GeneratorInstance;
	FVoxelGenerationData VoxelGenerationData;

	float** HeightMap;
	EBiomType** BiomMap;
	float** WaterMap;
	float*** UndergroundMap;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	UFUNCTION(BlueprintCallable)
	void PrintFullMap();
private:
	void GetFullSize();
	void XShift(int X);
	void YShift(int Y);
	void Diagonal(int X, int Y);
	void DeleteLine(int Index);
	void AddLine(bool IsForward);
	void AddColumn(bool isLeft);
	void AppendColumn(int Index, bool isLeft);
	void DeleteColumn(int Index);

	void InitializeParameters();
	void GenerateMaps();
	void GenerateHeightMap() const;
	void GenerateBiomMap() const;
	void GenerateCaveMap() const;

	AVoxelChank* SpawnChunk(float X, float Y, float Z);
	FIntVector GetPlayerChunkCoordinates();
};
