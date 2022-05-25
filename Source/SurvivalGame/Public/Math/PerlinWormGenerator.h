#pragma once

#include "Structures/FWormSettings.h"

class PerlinWormGenerator
{
public:
	PerlinWormGenerator(int SizeParam, int DepthParam, FWormSettings WormSettingsParam);
	void GenerateCaves(float*** UndergroundMapParam, float*** FirstNoiseParam, float*** SecondNoiseParam,float*** CavePositions);
private:
	void InitializeDirectionList();
	bool CheckNeighbours(int X, int Y, int Z, bool IsMaxima);
	FVector GetPerlinWormDirection();
	void FindLocalMaximas();
	void FindLocalMinimas();
	bool FailCondition(float FirstNoise, float NeighbourNoise, bool IsMaxima);
	void CreateWorm(FIntVector Maxim);
	FIntVector FindNearestMinimum(FIntVector CheckPoint);

	float*** UndergroundMap;
	float*** FirstNoise;
	float*** SecondNoise;
	float*** CavePositions;
	
	TArray<FIntVector>* Directions;
	TArray<FIntVector>* Maximas;
	TArray<FIntVector>* Minimas;
	int Size;
	int Depth;
	FWormSettings WormSettings;
};
