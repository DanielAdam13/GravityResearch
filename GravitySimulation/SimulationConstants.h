#pragma once

namespace gravity
{
	namespace constants
	{
		static constexpr double WorldPerSimUnit{ 200.0 };  // 1 normalized distance = 200 world units at zoom 1
		static constexpr int SimulationSteps{ 1024 };
		static constexpr int StepsPerPrimaryPeriod{ 1000 };

		inline constexpr double EarthMoonDistanceKm{ 384400.0 }; // Length unit of the PCR3BP
		inline constexpr double EarthRadiusKm{ 6378.137 };
		inline constexpr double MoonRadiusKm{ 1737.4 };
		inline constexpr double BodyDisplayExaggeration{ 10.0 };
		inline constexpr float  MinBodyDiameterWorld{ 8.f }; // Keeps tiny things visible

		static constexpr double EarthMu{ 398600.435507 };
		static constexpr double MoonMu{ 4902.800118 };
	}
}