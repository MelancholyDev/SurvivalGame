#pragma once

#include "CoreMinimal.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Structures/FVoxelGeneraionData.h"
#include "VoxelChank.generated.h"

UCLASS()
class SURVIVALGAME_API AVoxelChank : public AActor
{
	GENERATED_BODY()
	FVoxelGenerationData Data;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstanceTopGrass;
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstanceDirt;
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstanceStone;
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstanceSand;
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstanceSnow;
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* InstanceWater;
	
	void ActorLocationVoxelWorldXY(const int XIndex, const int YIndex, int& X, int& Y) const;
	void ActorLocationVoxelWorldZ(const int ZIndex, int& Z) const;
	bool CheckInBound(int Index,int Size);
	void AddSolidBlock(FTransform Position,EBiomType Biom);

public:
	AVoxelChank();
	virtual void OnConstruction(const FTransform& Transform) override;
	void InitializeParameters(FVoxelGenerationData DataParam);
};
