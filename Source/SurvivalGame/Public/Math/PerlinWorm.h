#pragma once
#include "Structures/FWormSettings.h"

class PerlinWorm
{
public:
	PerlinWorm(float*** UndergroundMapParam, float*** FirstNoiseParam,float*** SecondNoiseParam, FWormSettings WormSettingsParam,
	           FIntVector StartPositionPram, int SizeParam, int DepthParam);
	PerlinWorm(float*** UndergroundMapParam, float*** FirstNoiseParam,float*** SecondNoiseParam, FWormSettings WormSettingsParam,
			FIntVector StartPositionPram,FIntVector ConvergancePoint, int SizeParam, int DepthParam);
	void MoveLength(int Length);
private:
	FIntVector CurrentDirection;
	FIntVector CurrentPosition;
	FIntVector ConvergancePoint;
	bool IsMoveToConvergancePoint;
	float*** UndergroundMap;
	float*** FirstNoise;
	float*** SecondNoise;
	int Size;
	int Depth;
	int Width;
	float Weight;
	FWormSettings WormSetings;

	void Move();
	void MoveToConvergancePoint();
	void MarkEmpty(int X,int Y,int Z) const;
	FIntVector GetDirection();
};
