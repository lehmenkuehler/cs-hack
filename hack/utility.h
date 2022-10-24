#pragma once
#include <ctime>

namespace Utility
{
#pragma warning( disable : 4505)
	static int ClockDiff(std::clock_t t1, std::clock_t t2)
	{
		float diffms;
		if (t1 > t2) diffms = float(t1 - t2) / (CLOCKS_PER_SEC / 1000);
		else diffms = float(t2 - t1) / (CLOCKS_PER_SEC / 1000);
		return static_cast<int>(diffms);
	}
}