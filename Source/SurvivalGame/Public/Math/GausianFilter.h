#pragma once
#include "Structures/FGausianParameters.h"

class GausianFilter
{
public:
	GausianFilter(FGausianParameters Parameters, int Size);
	void SmoothMap(float** Map, float** FinalMap);
private:
	void CreateKernel();
	float Gausian(float X, float Mu);

	int MapSize;
	int KernelSize;
	float** Kernel;
	float Sigma;
};
