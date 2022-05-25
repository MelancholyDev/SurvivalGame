#pragma once
#include "Structures/FWaterParameters.h"

class WaterGenerator
{
public:
	WaterGenerator(FWaterParameters WaterParams);
private:
	FWaterParameters Water;
};
