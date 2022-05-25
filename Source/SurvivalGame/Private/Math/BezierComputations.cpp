#include "Math/BezierComputations.h"

#include "Point.h"
#include "vector"

float BezierComputations::BezierLut(float X)
{
	auto t = Linspace(0, 1, 256);
	std::vector<float> vecX{};
	std::vector<float> vecY{};
	for (int i = 0; i < t.size(); i++)
	{
		Point a = Bezier(t.at(i));
		vecX.push_back(a.X);
		vecY.push_back(a.Y);
	}
	const float result = Interpolation1(vecX, vecY, X);
	return result;
}

Point BezierComputations::Bezier( float T) const
{
	Point P1 = Point(0, 0);
	Point P2 = Point(X1, Y1);
	Point P3 = Point(X2, Y2);
	Point P4 = Point(1, A);
	Point Result = pow(1 - T, 3) * P1 + 3 * pow((1 - T), 2) * T * P2 + 3 * (1 - T) * pow(T, 2) * P3 + pow(T, 3) * P4;
	return Result;
}

BezierComputations::BezierComputations(TMap<EBiomType, FBiomData> BiomDataSet)
{
	DataSet=BiomDataSet;
	X1 =0;
	X2=0;
	Y1=0;
	Y2=0;
	A=0;
}

float BezierComputations::FilterMap(float HeightMap, float SmoothMap, TEnumAsByte<EBiomType> BiomData)
{
	FBiomData* Biom = DataSet.Find(BiomData);
	float B = Biom->B;
	X1 = Biom->X1;
	X2 = Biom->X2;
	Y1 = Biom->Y1;
	Y2 = Biom->Y2;
	A = Biom->A;
	float X = B * HeightMap + (1 - B) * SmoothMap;
	float Y = BezierLut(X);
	return Y;
}

void BezierComputations::CheckValue(float Value,TEnumAsByte<EBiomType> BiomData)
{
	FBiomData* Biom = DataSet.Find(BiomData);
	Biom->CheckValue(Value);
}

int BezierComputations::NearestNeighbourIndex(std::vector<float>& x, float& value)
{
	float Dist = std::numeric_limits<float>::max();
	float NewDist;
	size_t Idx = 0;

	for (size_t i = 0; i < x.size(); ++i)
	{
		NewDist = std::abs(value - x[i]);
		if (NewDist <= Dist)
		{
			Dist = NewDist;
			Idx = i;
		}
	}

	return Idx;
}

float BezierComputations::Interpolation1(std::vector<float>& X, std::vector<float>& Y, float& XNew)
{
	float YNew;
	float DX, DY, M, B;
	size_t XMaxIdx = X.size() - 1;
	size_t Idx = NearestNeighbourIndex(X, XNew);
	if (X[Idx] > XNew)
	{
		DX = Idx > 0 ? (X[Idx] - X[Idx - 1]) : (X[Idx + 1] - X[Idx]);
		DY = Idx > 0 ? (Y[Idx] - Y[Idx - 1]) : (Y[Idx + 1] - Y[Idx]);
	}
	else
	{
		DX = Idx < XMaxIdx ? (X[Idx + 1] - X[Idx]) : (X[Idx] - X[Idx - 1]);
		DY = Idx < XMaxIdx ? (Y[Idx + 1] - Y[Idx]) : (Y[Idx] - Y[Idx - 1]);
	}
	M = DY / DX;
	B = Y[Idx] - X[Idx] * M;
	YNew = XNew * M + B;

	return YNew;
}

std::vector<float> BezierComputations::Linspace(float StartIn, float EndIn, int NumIn)
{
	std::vector<float> Linspaced;

	double Start = StartIn;
	double End = EndIn;
	double Num = NumIn;

	if (Num == 0) { return Linspaced; }
	if (Num == 1)
	{
		Linspaced.push_back(Start);
		return Linspaced;
	}

	double Delta = (End - Start) / (Num - 1);

	for (int i = 0; i < Num - 1; ++i)
	{
		Linspaced.push_back(Start + Delta * i);
	}
	Linspaced.push_back(End);
	return Linspaced;
}

