#include "Classes/Generator.h"

#include "SimplexNoiseBPLibrary.h"
#include "Structures/FWormSettings.h"

Generator::Generator(FGenerationParameters Parameters, UDataTable* Table)
{
	WormSettings = Parameters.UndergroundParameters.WormSettings;
	UndergroundParameters = Parameters.UndergroundParameters;
	GausianParameters = Parameters.GausianParameters;
	PerlinNoiseParameters = Parameters.PerlinNoiseParameters;
	TemperatureParameters = Parameters.TemperatureAndMoistureParameters.TemperatureParameters;
	MoistureParameters = Parameters.TemperatureAndMoistureParameters.MoistureParameters;
	DiamondSquareParameters = Parameters.DiamondSquareParameters;
	GenerationParameters = Parameters;
	DataTableBiome = Table;
	InitializeBiomData();
	if (GenerationParameters.GenerationType == PERLIN_NOISE)
	{
		MapSize = (GenerationParameters.ChunkSize * 2 + 1) * PerlinNoiseParameters.MapSizeMultiplier;
	}
	else
	{
		MapSize = FMath::Pow(2, GenerationParameters.DiamondSquareParameters.MapMultiplier) + 1;
	}
	LeftBorder = -(MapSize - 1) / 2;
	RightBorder = -LeftBorder;

	BezierComputationsInstance = new BezierComputations(BiomDataSet);
	DiamondSquareInstance = new DiamondSquare(DiamondSquareParameters, MapSize);
	GausianFilterInstance = new GausianFilter(GausianParameters, MapSize);
	WormGenerator = new PerlinWormGenerator(MapSize, UndergroundParameters.CaveStart - UndergroundParameters.Depth + 1, WormSettings);
}

float Generator::Clamp(float x, float left, float right)
{
	if (x < left)
	{
		return -x;
	}
	if (x > right)
		return right;
	return x;
}

void Generator::GenerateHeightMap(float** Map, EBiomType** BiomMap)
{
	switch (GenerationParameters.GenerationType)
	{
	case PERLIN_NOISE:
		{
			GenerateWithPerlinNoise(Map, BiomMap);
		}
		break;
	case DIAMOND_SQUARE:
		{
			GenerateWithDiamondSquare(Map);
		}
		break;
	default: ;
	}
}

void Generator::GenerateBiomMaps(EBiomType** BiomMap)
{
	float** TemperatureMap = new float*[MapSize];
	float** MoistureMap = new float*[MapSize];

	for (int i = 0; i < MapSize; i++)
	{
		MoistureMap[i] = new float[MapSize];
		TemperatureMap[i] = new float[MapSize];
	}

	USimplexNoiseBPLibrary::createSeed(TemperatureParameters.Seed);
	USimplexNoiseBPLibrary::setNoiseSeed(TemperatureParameters.Seed);

	for (int i = LeftBorder; i <= RightBorder; i++)
		for (int j = RightBorder; j >= LeftBorder; j--)
		{
			float HeatNoise = USimplexNoiseBPLibrary::GetSimplexNoise2D_EX(
				i, -j, TemperatureParameters.Lacunarity, TemperatureParameters.Persistence,
				TemperatureParameters.Octaves, TemperatureParameters.NoiseDensity, TemperatureParameters.ZeroToOne);
			HeatNoise = Clamp(HeatNoise, 0, 1);
			const int XIndex = i + RightBorder;
			const int YIndex = j + RightBorder;

			TemperatureMap[XIndex][YIndex] = HeatNoise;
		}

	USimplexNoiseBPLibrary::createSeed(MoistureParameters.Seed);
	USimplexNoiseBPLibrary::setNoiseSeed(MoistureParameters.Seed);

	for (int i = LeftBorder; i <= RightBorder; i++)
		for (int j = RightBorder; j >= LeftBorder; j--)
		{
			float MoistureNoise = USimplexNoiseBPLibrary::GetSimplexNoise2D_EX(
				i, -j, MoistureParameters.Lacunarity, MoistureParameters.Persistence,
				MoistureParameters.Octaves, MoistureParameters.NoiseDensity, MoistureParameters.ZeroToOne);
			MoistureNoise = Clamp(MoistureNoise, 0, 1);

			const int XIndex = i + RightBorder;
			const int YIndex = j + RightBorder;

			MoistureMap[XIndex][YIndex] = MoistureNoise;
		}

	for (int i = 0; i < MapSize; i++)
		for (int j = 0; j < MapSize; j++)
		{
			float MoistureNoise = MoistureMap[i][j];
			float TemperatureNoise = TemperatureMap[i][j];

			BiomMap[i][j] = (EBiomType)GetBiom(TemperatureNoise, MoistureNoise);
		}
}


void Generator::GenerateSeaMap(float** Map)
{
}

void Generator::GenerateCaveMap(float*** UndergroundMap)
{
	int Depth = UndergroundParameters.CaveStart - UndergroundParameters.Depth + 1;
	float*** FirstNoise = new float**[MapSize];
	float*** SecondNoise = new float**[MapSize];
	float*** CavePositions = new float**[MapSize];

	for (int i = 0; i < MapSize; i++)
	{
		FirstNoise[i] = new float*[MapSize];
		CavePositions[i] = new float*[MapSize];
		SecondNoise[i] = new float*[MapSize];
		for (int j = 0; j < MapSize; j++)
		{
			FirstNoise[i][j] = new float[Depth];
			CavePositions[i][j] = new float[Depth];
			SecondNoise[i][j] = new float[Depth];
		}
	}
	USimplexNoiseBPLibrary::createSeed(UndergroundParameters.FirstNoise.Seed);
	USimplexNoiseBPLibrary::setNoiseSeed(UndergroundParameters.FirstNoise.Seed);

	for (int i = LeftBorder; i <= RightBorder; i++)
		for (int j = RightBorder; j >= LeftBorder; j--)
		{
			int DepthIndex = 0;
			for (int k = UndergroundParameters.CaveStart; k >= UndergroundParameters.Depth; k--)
			{
				float Noise3D = USimplexNoiseBPLibrary::GetSimplexNoise3D_EX(
					i, -j, k, UndergroundParameters.FirstNoise.Lacunarity, UndergroundParameters.FirstNoise.Persistence,
					UndergroundParameters.FirstNoise.Octaves,
					UndergroundParameters.FirstNoise.NoiseDensity, UndergroundParameters.FirstNoise.ZeroToOne);
				const int XIndex = i + RightBorder;
				const int YIndex = j + RightBorder;
				FirstNoise[XIndex][YIndex][DepthIndex] = Clamp(Noise3D, 0, 1);
				DepthIndex++;
			}
		}

	USimplexNoiseBPLibrary::createSeed(UndergroundParameters.SecondNoise.Seed);
	USimplexNoiseBPLibrary::setNoiseSeed(UndergroundParameters.SecondNoise.Seed);

	for (int i = LeftBorder; i <= RightBorder; i++)
		for (int j = RightBorder; j >= LeftBorder; j--)
		{
			int DepthIndex = 0;
			for (int k = UndergroundParameters.CaveStart; k >= UndergroundParameters.Depth; k--)
			{
				float Noise3D = USimplexNoiseBPLibrary::GetSimplexNoise3D_EX(
					i, -j, k, UndergroundParameters.SecondNoise.Lacunarity, UndergroundParameters.SecondNoise.Persistence,
					UndergroundParameters.SecondNoise.Octaves,
					UndergroundParameters.SecondNoise.NoiseDensity, UndergroundParameters.SecondNoise.ZeroToOne);
				const int XIndex = i + RightBorder;
				const int YIndex = j + RightBorder;
				SecondNoise[XIndex][YIndex][DepthIndex] = Clamp(Noise3D, 0, 1);
				DepthIndex++;
			}
		}
	USimplexNoiseBPLibrary::createSeed(UndergroundParameters.WormPlaceNoise.Seed);
	USimplexNoiseBPLibrary::setNoiseSeed(UndergroundParameters.WormPlaceNoise.Seed);

	for (int i = LeftBorder; i <= RightBorder; i++)
		for (int j = RightBorder; j >= LeftBorder; j--)
		{
			int DepthIndex = 0;
			for (int k = UndergroundParameters.CaveStart; k >= UndergroundParameters.Depth; k--)
			{
				float Noise3D = USimplexNoiseBPLibrary::GetSimplexNoise3D_EX(
					i, -j, k, UndergroundParameters.WormPlaceNoise.Lacunarity, UndergroundParameters.WormPlaceNoise.Persistence,
					UndergroundParameters.WormPlaceNoise.Octaves,
					UndergroundParameters.WormPlaceNoise.NoiseDensity, UndergroundParameters.WormPlaceNoise.ZeroToOne);
				const int XIndex = i + RightBorder;
				const int YIndex = j + RightBorder;
				CavePositions[XIndex][YIndex][DepthIndex] = Clamp(Noise3D, 0, 1);
				DepthIndex++;
			}
		}

	WormGenerator->GenerateCaves(UndergroundMap, FirstNoise, SecondNoise, CavePositions);
}

void Generator::InitializeBiomData()
{
	TArray<FName> RowNames = DataTableBiome->GetRowNames();
	FString EmptyString;
	for (auto& Name : RowNames)
	{
		FBiomData Data = *DataTableBiome->FindRow<FBiomData>(Name, EmptyString);
		BiomDataSet.Add(Data.Type, Data);
	}
}

void Generator::GenerateWithDiamondSquare(float** Map)
{
	DiamondSquareInstance->GenerateMap(Map);
	if (GenerationParameters.IsApplyGausianFilter)
	{
		float** TempMap = new float*[MapSize];
		for (int i = 0; i < MapSize; i++)
		{
			TempMap[i] = new float[MapSize];
			for (int j = 0; j < MapSize; j++)
				TempMap[i][j] = Map[i][j];
		}
		GausianFilterInstance->SmoothMap(TempMap, Map);
	}
}

uint8 Generator::GetBiom(float Heat, float Moisture)
{
	uint8 bytes;
	if (Heat < 0.33)
	{
		if (Moisture < 0.33)
		{
			bytes = (uint8)BOREAL;
		}
		else if ((Moisture >= 0.33) & (Moisture < 0.66))
		{
			bytes = (uint8)SEASONAL_FOREST;
		}
		else
		{
			bytes = (uint8)TUNDRA;
		}
	}
	else if ((Heat >= 0.33) & (Heat < 0.66))
	{
		if (Moisture < 0.33)
		{
			bytes = (uint8)TEMPERATE_FOREST;
		}
		else if ((Moisture >= 0.33) & (Moisture < 0.66))
		{
			bytes = (uint8)TROPICAL_WOODLAND;
		}
		else
		{
			bytes = (uint8)TEMPERATE_RAINFOREST;
		}
	}
	else
	{
		if (Moisture < 0.33)
		{
			bytes = (uint8)DESERT;
		}
		else if ((Moisture >= 0.33) & (Moisture < 0.66))
		{
			bytes = (uint8)SAVANNA;
		}
		else
		{
			bytes = (uint8)RAIN_FOREST;
		}
	}
	return bytes;
}

void Generator::GenerateWithPerlinNoise(float** Map, EBiomType** BiomMap)
{
	float** TempHeightMap = new float*[MapSize];
	for (int i = 0; i < MapSize; i++)
	{
		TempHeightMap[i] = new float[MapSize];
	}
	USimplexNoiseBPLibrary::createSeed(PerlinNoiseParameters.Sharp.Seed);
	USimplexNoiseBPLibrary::setNoiseSeed(PerlinNoiseParameters.Sharp.Seed);
	for (int i = LeftBorder; i <= RightBorder; i++)
		for (int j = RightBorder; j >= LeftBorder; j--)
		{
			const int XIndex = i + RightBorder;
			const int YIndex = j + RightBorder;
			float SharpNoise;
			float SmoothNoise;

			SharpNoise = USimplexNoiseBPLibrary::GetSimplexNoise2D_EX(
				i, -j, PerlinNoiseParameters.Sharp.Lacunarity, PerlinNoiseParameters.Sharp.Persistence,
				PerlinNoiseParameters.Sharp.Octaves,
				PerlinNoiseParameters.Sharp.NoiseDensity, PerlinNoiseParameters.Sharp.ZeroToOne);


			SmoothNoise = USimplexNoiseBPLibrary::GetSimplexNoise2D_EX(
				i, -j, PerlinNoiseParameters.Smooth.Lacunarity, PerlinNoiseParameters.Smooth.Persistence,
				PerlinNoiseParameters.Smooth.Octaves,
				PerlinNoiseParameters.Smooth.NoiseDensity, PerlinNoiseParameters.Smooth.ZeroToOne);

			SmoothNoise = Clamp(SmoothNoise, 0, 1);
			SharpNoise = Clamp(SharpNoise, 0, 1);

			TEnumAsByte<EBiomType> CurrentBiom;
			if (PerlinNoiseParameters.IsTest)
			{
				CurrentBiom = BiomMap[XIndex][YIndex];
			}
			else
			{
				CurrentBiom = PerlinNoiseParameters.DefaultBiom;
			}
			float FinalNoise = BezierComputationsInstance->FilterMap(SharpNoise, SmoothNoise, CurrentBiom);
			float Clamped = Clamp(FinalNoise, 0, 1);
			BezierComputationsInstance->CheckValue(Clamped, CurrentBiom);
			TempHeightMap[XIndex][YIndex] = Clamped;
		}

	if (PerlinNoiseParameters.IsInvert)
	{
		InvertMap(TempHeightMap, BiomMap);
	}
	for (int i = 0; i < MapSize; i++)
	{
		for (int j = 0; j < MapSize; j++)
		{
			Map[i][j] = TempHeightMap[i][j];
		}
	}
	if (GenerationParameters.IsApplyGausianFilter)
	{
		GausianFilterInstance->SmoothMap(TempHeightMap, Map);
	}
}

void Generator::InvertMap(float** MapForInvert, EBiomType** BiomMap)
{
	for (int i = LeftBorder; i <= RightBorder; i++)
		for (int j = RightBorder; j >= LeftBorder; j--)
		{
			const int XIndex = i + RightBorder;
			const int YIndex = j + RightBorder;
			TEnumAsByte<EBiomType> CurrentBiom;
			if (PerlinNoiseParameters.IsTest)
			{
				CurrentBiom = BiomMap[XIndex][YIndex];
			}
			else
			{
				CurrentBiom = PerlinNoiseParameters.DefaultBiom;
			}
			FBiomData* CurrentBiomData = BezierComputationsInstance->DataSet.Find(CurrentBiom);
			float Noise = MapForInvert[XIndex][YIndex];
			Noise = CurrentBiomData->Max - (Noise - CurrentBiomData->Min);
			MapForInvert[XIndex][YIndex] = Noise;
		}
}
