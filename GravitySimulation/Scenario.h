#pragma once
#include "PhysicsCore.h"

#include <glm/glm.hpp>
#include <cmath>

namespace gravity
{
	struct Scenario
	{
		double r0;
		double phase; // angle from Earth-Moon line
	};

	// Two-body start - Earth-centred inertial frame
	// -> pos = r0*(cos, sin)
	// -> vel = vc*(-sin, cos)
	State<double> TwoBodyStart(const Scenario& sc, double massRatio)
	{
		// vc = sqrt((1-mu)/r0)
		const double vc{ std::sqrt((1.0 - massRatio) / sc.r0) }; // Uses earth's grav param: 1 - mu

		const glm::dvec2 pos{ sc.r0 * std::cos(sc.phase), sc.r0 * std::sin(sc.phase) };
		const glm::dvec2 vel{ -vc * std::sin(sc.phase), vc * std::cos(sc.phase) };

		return { pos, vel };
	}


	// PCR3BP start - origin at barycenter!!!
	// -> pos = r0 * (cos, sin)
	// -> vel = vc * (-sin, cos)
	State<double> Pcr3bpStart(const Scenario& sc, double massRatio)
	{
		const State<double> relativeStart{ TwoBodyStart(sc, massRatio) };

		// x = r0 * cos(phase) - mu,  y = r0 * sin(phase)
		const glm::dvec2 pos{ relativeStart.position.x - massRatio, relativeStart.position.y };

		// vx = -vc * sin(phase) + posy, vy = vc * cos(phase) - (posx + mu)
		const glm::dvec2 vel{ relativeStart.velocity.x + pos.y, relativeStart.velocity.y - (pos.x + massRatio) };
	}
}
