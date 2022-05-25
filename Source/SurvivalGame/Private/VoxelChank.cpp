#include "VoxelChank.h"
#include "Math/BezierComputations.h"
#include "SimplexNoiseBPLibrary.h"
#include "Chaos/KinematicTargets.h"

//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%f"),Shift));

AVoxelChank::AVoxelChank()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	Root->SetMobility(EComponentMobility::Static);

	InstanceTopGrass = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstanceGrass"));
	InstanceTopGrass->SetMobility(EComponentMobility::Static);
	InstanceTopGrass->SetupAttachment(Root);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Grass(
		TEXT("StaticMesh'/Game/SurvivalGeneration/Models/Meshes/EarthGrass.EarthGrass'"));
	InstanceTopGrass->SetStaticMesh(Grass.Object);
	InstanceTopGrass->SetRelativeLocation(FVector(0, 0, 0));

	InstanceDirt = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstanceDirt"));
	InstanceDirt->SetMobility(EComponentMobility::Static);
	InstanceDirt->SetupAttachment(Root);
	InstanceDirt->SetRelativeLocation(FVector(0, 0, 0));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Dirt(
		TEXT("StaticMesh'/Game/SurvivalGeneration/Models/Meshes/Dirt.Dirt'"));
	InstanceDirt->SetStaticMesh(Dirt.Object);

	InstanceSnow = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstanceSnow"));
	InstanceSnow->SetMobility(EComponentMobility::Static);
	InstanceSnow->SetupAttachment(Root);
	InstanceSnow->SetRelativeLocation(FVector(0, 0, 0));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Snow(
		TEXT("StaticMesh'/Game/SurvivalGeneration/Models/Meshes/Snow.Snow'"));
	InstanceSnow->SetStaticMesh(Snow.Object);

	InstanceSand = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstanceSand"));
	InstanceSand->SetMobility(EComponentMobility::Static);
	InstanceSand->SetupAttachment(Root);
	InstanceSand->SetRelativeLocation(FVector(0, 0, 0));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Sand(
		TEXT("StaticMesh'/Game/SurvivalGeneration/Models/Meshes/Sand.Sand'"));
	InstanceSand->SetStaticMesh(Sand.Object);

	InstanceStone = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstanceStone"));
	InstanceStone->SetMobility(EComponentMobility::Static);
	InstanceStone->SetupAttachment(Root);
	InstanceStone->SetRelativeLocation(FVector(0, 0, 0));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Stone(
		TEXT("StaticMesh'/Game/SurvivalGeneration/Models/Meshes/Stone.Stone'"));
	InstanceStone->SetStaticMesh(Stone.Object);

	InstanceWater = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstanceWater"));
	InstanceWater->SetMobility(EComponentMobility::Static);
	InstanceWater->SetupAttachment(Root);
	InstanceWater->SetRelativeLocation(FVector(0, 0, 0));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Water(
		TEXT("StaticMesh'/Game/SurvivalGeneration/Models/Meshes/Water.Water'"));
	InstanceWater->SetStaticMesh(Water.Object);
	InstanceWater->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AVoxelChank::OnConstruction(const FTransform& Transform)
{
	InstanceTopGrass->ClearInstances();
	InstanceDirt->ClearInstances();
	InstanceStone->ClearInstances();

	for (int LoopX = Data.ChunkSize * -1; LoopX <= Data.ChunkSize; LoopX++)
	{
		for (int LoopY = Data.ChunkSize * -1; LoopY <= Data.ChunkSize; LoopY++)
		{
			int A;
			int B;
			int IndexShift = Data.MapSize / 2;
			ActorLocationVoxelWorldXY(LoopX, LoopY, A, B);
			int XIndex = A / Data.VoxelSize + IndexShift;
			int YIndex = B / Data.VoxelSize + IndexShift;
			float HeightNoise;
			EBiomType SpawnBiom;
			if (CheckInBound(XIndex, Data.MapSize) & CheckInBound(YIndex, Data.MapSize))
			{
				HeightNoise = Data.Map[XIndex][YIndex];
				SpawnBiom = Data.BiomMap[XIndex][YIndex];
			}
			else
			{
				HeightNoise = 1;
				SpawnBiom = TUNDRA;
			}

			HeightNoise = HeightNoise * Data.NoiseScale;
			int CurrentMapCoordinate = floor(HeightNoise);
			int ShiftClamped = CurrentMapCoordinate * Data.VoxelSize;
			FVector SpawnPosition(LoopX * Data.VoxelSize, LoopY * Data.VoxelSize, ShiftClamped);
			FTransform SpawnTransform = FTransform(FRotator(0, 0, 0), SpawnPosition, FVector(0.5, 0.5, 0.5));
			if (HeightNoise > Data.WaterLevel)
			{
				AddSolidBlock(SpawnTransform,SpawnBiom);
			}
			else
			{
				InstanceWater->AddInstance(SpawnTransform);
			}
			if (Data.IsAddDepth)
			{
				int DepthCount = -(CurrentMapCoordinate - Data.Depth);
				int DepthIterator = 0;
				for (int i = -1; i >= DepthCount; i--)
				{
					int DepthCoordinate = CurrentMapCoordinate + i;
					SpawnPosition.Z = ShiftClamped + i * Data.VoxelSize;
					int A1, B1, C1;
					ActorLocationVoxelWorldXY(LoopX, LoopY, A1, B1);
					ActorLocationVoxelWorldZ(i, C1);
					if (DepthCoordinate > Data.CaveStart)
					{
						float Noise3D = USimplexNoiseBPLibrary::SimplexNoise3D(
							XIndex, YIndex, DepthCoordinate, Data.NoiseDensity3D);
						if ((Noise3D > Data.Threshold3D) || (i > -3) || ((i - 1) == DepthCount))
						{
							SpawnTransform = FTransform(FRotator(0, 0, 0), SpawnPosition, FVector(0.5, 0.5, 0.5));
							InstanceDirt->AddInstance(SpawnTransform);
						}
					}
					else
					{
						float Noise3D = Data.UndergroundMap[XIndex][YIndex][DepthIterator];
						DepthIterator++;
						if (Noise3D ==0)
						{
							SpawnTransform = FTransform(FRotator(0, 0, 0), SpawnPosition, FVector(0.5, 0.5, 0.5));
							InstanceStone->AddInstance(SpawnTransform);
						}
					}
				}
			}
		}
	}
}

void AVoxelChank::InitializeParameters(FVoxelGenerationData DataParam)
{
	Data = DataParam;
}

void AVoxelChank::ActorLocationVoxelWorldXY(const int XIndex, const int YIndex, int& X, int& Y) const
{
	const FVector Location = InstanceTopGrass->GetComponentLocation();
	X = Location.X + XIndex * Data.VoxelSize;
	Y = Location.Y + YIndex * Data.VoxelSize;
}

void AVoxelChank::ActorLocationVoxelWorldZ(const int ZIndex, int& Z) const
{
	const FVector Location = InstanceTopGrass->GetComponentLocation();
	Z = Location.Z + ZIndex * Data.VoxelSize;
}

bool AVoxelChank::CheckInBound(int Index, int Size)
{
	if ((Index < Size) & (Index >= 0))
		return true;
	return false;
}

void AVoxelChank::AddSolidBlock(FTransform SpawnTransform, EBiomType Biom)
{
	if ((Biom == TUNDRA) | (Biom==BOREAL) | (Biom==SEASONAL_FOREST))
	{
		InstanceSnow->AddInstance(SpawnTransform);
	}
	else if ((Biom == TROPICAL_WOODLAND) | (Biom == TEMPERATE_FOREST) | (Biom == TEMPERATE_RAINFOREST))
	{
		InstanceTopGrass->AddInstance(SpawnTransform);
	}
	else
	{
		InstanceSand->AddInstance(SpawnTransform);
	}
}
