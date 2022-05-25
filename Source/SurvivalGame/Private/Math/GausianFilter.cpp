#include "Math/GausianFilter.h"

#include <valarray>

using namespace std;

void GausianFilter::CreateKernel()
{
	float Sum = 0;
	Kernel = new float*[KernelSize];
	for (int i = 0; i < KernelSize; i++)
		Kernel[i] = new float[KernelSize];
	float KernelRadius = (KernelSize - 1) / 2;
	for (int i = 0; i < KernelSize; i++)
		for (int j = 0; j < KernelSize; j++)
		{
			double x = Gausian(i, KernelRadius)
				* Gausian(j, KernelRadius);
			Kernel[i][j] = x;
			Sum += x;
		}
	for (int i = 0; i < KernelSize; i++)
		for (int j = 0; j < KernelSize; j++)
		{
			Kernel[i][j] /= Sum;
		}
}


float GausianFilter::Gausian(float X, float Mu)
{
	const float A = (X - Mu) / Sigma;
	return std::exp(-0.5 * A * A);
}

GausianFilter::GausianFilter(FGausianParameters Parameters, int Size)
{
	Kernel = nullptr;
	KernelSize = Parameters.KernelSize;
	Sigma = Parameters.Sigma;
	MapSize = Size;
	CreateKernel();
}

void GausianFilter::SmoothMap(float** Map, float** FinalMap)
{
	float** PixelMap = new float*[KernelSize];
	for (int i = 0; i < KernelSize; i++)
	{
		PixelMap[i] = new float[KernelSize];
	}
	const int Border = (KernelSize - 1) / 2;
	for (int i = 0; i < MapSize; i++)
		for (int j = 0; j < MapSize; j++)
		{
			for (int k = 0; k < KernelSize; k++)
			{
				for (int l = 0; l < KernelSize; l++)
				{
					int XIndex = i + k - Border;
					int YIndex = j + l - Border;
					if ((XIndex > 0) & (XIndex < MapSize) & (YIndex > 0) & (YIndex < MapSize))
					{
						PixelMap[k][l] = Map[i + k - Border][j + l - Border] * Kernel[k][l];
					}
					else
					{
						PixelMap[k][l] = -1;
					}
				}
			}
			float Sum = 0;
			for (int k = 0; k < KernelSize; k++)
				for (int l = 0; l < KernelSize; l++)
					Sum += PixelMap[k][l] < 0 ? 0 : PixelMap[k][l];

			FinalMap[i][j] = Sum;
		}
}
