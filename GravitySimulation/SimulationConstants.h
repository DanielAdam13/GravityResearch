#pragma once

namespace gravity
{
	namespace constants
	{
		static constexpr double WorldPerSimUnit{ 200.0 };  // 1 normalized distance = 200 world units at zoom 1
		static constexpr int SimulationSteps{ 1024 };
		static constexpr int StepsPerPrimaryPeriod{ 1000 };

		static constexpr double EarthMu{ 398600.435507 };
		static constexpr double MoonMu{ 4902.800118 };
	}
}