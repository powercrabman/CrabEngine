#include "CrabEnginePch.h"
#include "CrabMath.h"
#include <random>

namespace crab::math
{
	namespace
	{
		std::random_device gRendomDevice;
		std::mt19937 gRendomGenerator(gRendomDevice());
	}

	int32 RandInt32(int32 low, int32 high)
	{
		std::uniform_int_distribution<int32> dis(low, high);
		return dis(gRendomGenerator);
	}

	float RandFloat(float low, float high)
	{
		std::uniform_real_distribution<float> dis(low, high);
		return dis(gRendomGenerator);
	}
}
