#include "Math/PerlinWorm.h"

PerlinWorm::PerlinWorm(float*** UndergroundMapParam, float*** FirstNoiseParam, float*** SecondNoiseParam,
                       FWormSettings WormSettingsParam,
                       FIntVector StartPositionParam, int SizeParam, int DepthParam)
{
	Width = WormSettingsParam.WormWidth - 1;
	Size = SizeParam;
	Depth = DepthParam;
	UndergroundMap = UndergroundMapParam;
	FirstNoise = FirstNoiseParam;
	SecondNoise = SecondNoiseParam;
	WormSetings = WormSettingsParam;
	CurrentPosition = StartPositionParam;
	IsMoveToConvergancePoint = false;
}

PerlinWorm::PerlinWorm(float*** UndergroundMapParam, float*** FirstNoiseParam, float*** SecondNoiseParam,
                       FWormSettings WormSettingsParam, FIntVector StartPositionParam, FIntVector ConvergancePointParam,
                       int SizeParam,
                       int DepthParam)
{
	Width = WormSettingsParam.WormWidth - 1;
	Size = SizeParam;
	Depth = DepthParam;
	UndergroundMap = UndergroundMapParam;
	FirstNoise = FirstNoiseParam;
	SecondNoise = SecondNoiseParam;
	WormSetings = WormSettingsParam;
	CurrentPosition = StartPositionParam;
	ConvergancePoint = ConvergancePointParam;
	IsMoveToConvergancePoint = true;
	Weight = WormSettingsParam.Weight;
}

void PerlinWorm::MoveLength(int Length)
{
	const int CurrentDirectionX = 1;
	const int CurrentDirectionY = 0;
	const int CurrentDirectionZ = 0;
	CurrentDirection = FIntVector(CurrentDirectionX, CurrentDirectionY, CurrentDirectionZ);
	FVector ConvergancePointFloat = FVector(ConvergancePoint);
	for (int i = 0; i < Length; i++)
	{
		if (IsMoveToConvergancePoint)
		{
			MoveToConvergancePoint();
			FVector CurrentPositionFloat = FVector(CurrentPosition);
			if (FVector::Distance(ConvergancePointFloat, CurrentPositionFloat) < 1)
				break;
		}
		else
		{
			Move();
		}
	}
	if (IsMoveToConvergancePoint)
	{
		int Iterator = 0;
		Weight = 0.9f;
		FVector CurrentPositionFloat = FVector(CurrentPosition);
		while (FVector::Distance(CurrentPositionFloat, ConvergancePointFloat) > 1)
		{
			MoveToConvergancePoint();
			Iterator++;
			if (Iterator > 50)
				break;
		}
	}
}

void PerlinWorm::Move()
{
	const FIntVector Direction = GetDirection();
	const FIntVector NextPosition = FIntVector(CurrentPosition.X + Direction.X, CurrentPosition.Y + Direction.Y,
	                                           CurrentPosition.Z + Direction.Z);
	if ((NextPosition.X >= 0) & (NextPosition.Y >= 0) & (NextPosition.Z >= 0) & (NextPosition.X < Size) & (NextPosition.
		Y < Size) & (NextPosition.Z < Depth))
	{
		MarkEmpty(NextPosition.X, NextPosition.Y, NextPosition.Z);
	}
	CurrentPosition = NextPosition;
	CurrentDirection = Direction;
}

void PerlinWorm::MoveToConvergancePoint()
{
	float Random = FMath::SRand();
	FIntVector Direction;
	if (Random < Weight)
	{
		Direction = GetDirection();
	}
	else
	{
		FIntVector Shift = FIntVector(ConvergancePoint.X - CurrentPosition.X, ConvergancePoint.Y - CurrentPosition.Y,
		                              ConvergancePoint.Z - CurrentPosition.Z);
		Direction = FIntVector(Shift.X / (Shift.X == 0 ? 1 : FMath::Abs(Shift.X)),
		                       Shift.Y / (Shift.Y == 0 ? 1 : FMath::Abs(Shift.Y)),
		                       Shift.Z / (Shift.Z == 0 ? 1 : FMath::Abs(Shift.Z)));
	}
	const FIntVector NextPosition = FIntVector(CurrentPosition.X + Direction.X, CurrentPosition.Y + Direction.Y,
	                                           CurrentPosition.Z + Direction.Z);
	if ((NextPosition.X >= 0) & (NextPosition.Y >= 0) & (NextPosition.Z >= 0) & (NextPosition.X < Size) & (
		NextPosition.
		Y < Size) & (NextPosition.Z < Depth))
	{
		MarkEmpty(NextPosition.X, NextPosition.Y, NextPosition.Z);
	}
	CurrentPosition = NextPosition;
	CurrentDirection = Direction;
}

void PerlinWorm::MarkEmpty(int X, int Y, int Z) const
{
	for (int i = X - Width; i <= X + Width; i++)
		for (int j = Y - Width; j <= Y + Width; j++)
			for (int k = Z - Width; k <= Z + Width; k++)
			{
				if ((i >= 0) & (j >= 0) & (k >= 0) & (i < Size) & (j < Size) & (k < Depth))
					UndergroundMap[i][j][k] = 0;
			}
}

FIntVector PerlinWorm::GetDirection()
{
	float FirstNoiseCurrent;
	float SecondNoiseCurrent;
	if ((CurrentPosition.X >= 0) & (CurrentPosition.Y >= 0) & (CurrentPosition.Z >= 0) & (CurrentPosition.X < Size) & (
		CurrentPosition.
		Y < Size) & (CurrentPosition.Z < Depth))
	{
		FirstNoiseCurrent = FirstNoise[CurrentPosition.X][CurrentPosition.Y][CurrentPosition.Z];
		SecondNoiseCurrent = SecondNoise[CurrentPosition.X][CurrentPosition.Y][CurrentPosition.Z];
	}
	else
	{
		return FIntVector(0, 0, 0);
	}
	float Angle;
	if ((FirstNoiseCurrent >= 0) & (FirstNoiseCurrent < 0.2))
	{
		Angle = -90;
	}
	else if ((FirstNoiseCurrent >= 0.2) & (FirstNoiseCurrent < 0.4))
	{
		Angle = -45;
	}
	else if ((FirstNoiseCurrent >= 0.4) & (FirstNoiseCurrent < 0.6))
	{
		Angle = 0;
	}
	else if ((FirstNoiseCurrent >= 0.6) & (FirstNoiseCurrent < 0.8))
	{
		Angle = 45;
	}
	else
	{
		Angle = 90;
	}

	const float AngleInRadians = Angle * PI / 180;

	const float ComputedX = FMath::Cos(AngleInRadians) * CurrentDirection.X - FMath::Sin(AngleInRadians) *
		CurrentDirection.Y;
	const float ComputedY = FMath::Sin(AngleInRadians) * CurrentDirection.X + FMath::Cos(AngleInRadians) *
		CurrentDirection.Y;

	FVector NextDirection = FVector(ComputedX, ComputedY, 0);
	if ((Angle == -45) | (Angle == 45))
	{
		if (NextDirection.Size() > 1)
		{
			NextDirection.Normalize();
		}
		else
		{
			NextDirection.X = NextDirection.X / FMath::Sqrt(2) * 2;
			NextDirection.Y = NextDirection.Y / FMath::Sqrt(2) * 2;
		}
	}
	FIntVector NextDirectionInt = FIntVector(FMath::Floor(NextDirection.X + 0.1), FMath::Floor(NextDirection.Y + 0.1),
	                                         0);
	int Z;
	if ((SecondNoiseCurrent >= 0) & (SecondNoiseCurrent < 0.33))
	{
		Z = -1;
	}
	else if ((SecondNoiseCurrent >= 0.33) & (SecondNoiseCurrent < 0.66))
	{
		Z = 0;
	}
	else
	{
		Z = 1;
	}
	NextDirectionInt.Z = Z;
	return NextDirectionInt;
}
