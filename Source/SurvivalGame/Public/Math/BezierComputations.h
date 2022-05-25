#pragma once
#include "Point.h"
#include "vector"
#include"Structures/FBiomData.h"

using namespace std;

class BezierComputations
{
public:
	BezierComputations(TMap<EBiomType, FBiomData> BiomDataSet);
	float FilterMap(float HeightMap, float SmoothMap, TEnumAsByte<EBiomType> BiomData);
	void CheckValue(float Value, TEnumAsByte<EBiomType> BiomData);
	TMap<EBiomType, FBiomData> DataSet;
private:
	float BezierLut(float X);
	Point Bezier(float T) const;
	int NearestNeighbourIndex(std::vector<float>& X, float& Value);
	float Interpolation1(std::vector<float>& X, std::vector<float>& Y, float& XNew);
	static std::vector<float> Linspace(float StartIn, float EndIn, int NumIn);
	float X1;
	float Y1;
	float X2;
	float Y2;
	float A;
};
