#pragma once

#include <random>

namespace random
{
	static std::mt19937 s_RandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
	
	static float Float()
	{
		s_RandomEngine.seed(std::random_device()());
		
		return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}
}