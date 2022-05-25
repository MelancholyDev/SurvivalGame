#include "DiamondSquareMapGeneration.h"
using namespace std;


void DiamondSquareMapGeneration::generateWorld(float** tiles, int worldSize)
{
	for (int i = 0; i < worldSize; i++)
		for (int j = 0; j < worldSize; j++)
			tiles[i][j] = 0;
	FMath::SRandInit(1);
	const float SEED = 0.0f;
	tiles[0][0] = SEED;
	tiles[0][worldSize - 1] = SEED;
	tiles[worldSize - 1][worldSize - 1] = SEED;
	tiles[worldSize - 1][0] = SEED;


	float h = 0.75;
	//int sideLength = worldSize - 1;
	for (int sideLength = worldSize - 1; sideLength >= 2; sideLength /= 2, h /= 2.0)
	{
		int halfSide = sideLength / 2;

		for (int x = 0; x < worldSize - 1; x += sideLength)
		{
			for (int y = 0; y < worldSize - 1; y += sideLength)
			{
				float avg = tiles[x][y] + tiles[x + sideLength][y] + tiles[x][y + sideLength] + tiles[x + sideLength][y
					+ sideLength];
				avg /= 4.0;

				tiles[x + halfSide][y + halfSide] = (avg + (fRand2(0, 1) * 2 * h) - h);
			}
		}

		for (int x = 0; x < worldSize - 1; x += halfSide)
		{
			for (int y = (x + halfSide) % sideLength; y < worldSize - 1; y += sideLength)
			{
				float avg =
					tiles[(x - halfSide + worldSize - 1) % (worldSize - 1)][y] + //left of center
					tiles[(x + halfSide) % (worldSize - 1)][y] + //right of center
					tiles[x][(y + halfSide) % (worldSize - 1)] + //below center
					tiles[x][(y - halfSide + worldSize - 1) % (worldSize - 1)]; //above center
				avg /= 4.0;
				float Rand = fRand2(0, 1);
				avg = (avg + (Rand * 2 * h) - h);
				tiles[x][y] = avg;
				if (x == 0) tiles[worldSize - 1][y] = avg;
				if (y == 0) tiles[x][worldSize - 1] = avg;
			}
		}
	}
}

float DiamondSquareMapGeneration::fRand2(float fMin, float fMax)
{
	float Diff = fMax - fMin;
	float Multiplier = FMath::SRand();
	return fMin + Diff * Multiplier;
}
