#pragma once
#include "PhysicsModels.h"

#include <glm/glm.hpp>
#include <cmath>
#include <numbers>

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
	inline State<double> TwoBodyStart(const Scenario& sc, double massRatio)
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
	inline State<double> Pcr3bpStart(const Scenario& sc, double massRatio)
	{
		const State<double> relativeStart{ TwoBodyStart(sc, massRatio) };

		// x = r0 * cos(phase) - mu,  y = r0 * sin(phase)
		const glm::dvec2 pos{ relativeStart.position.x - massRatio, relativeStart.position.y };

		// vx = -vc * sin(phase) + posy, vy = vc * cos(phase) - (posx + mu)
		const glm::dvec2 vel{ relativeStart.velocity.x + pos.y, relativeStart.velocity.y - (pos.x + massRatio) };

		return { pos, vel };
	}

	// Comverting a PCR3BP state to Earth-Centered coordinates
	// (Redoing Pcr3bpStart's output to its initial relativeStart)
	inline State<double> RotatingToEathCentered(const State<double>& state, double massRatio, double t)
	{
		const double px{ state.position.x + massRatio };
		const double py{ state.position.y };
		const double vx{ state.velocity.x - state.position.y };
		const double vy{ state.velocity.y + state.position.x + massRatio };

		const glm::dvec2 pos{ std::cos(t) * px - std::sin(t) * py, std::sin(t) * px + std::cos(t) * py };
		const glm::dvec2 vel{ std::cos(t) * vx - std::sin(t) * vy, std::sin(t) * vx + std::cos(t) * vy };

		return { pos,  vel };
	}

	template <typename To, typename From>
	State<To> CastState(const State<From>& s)
	{
		return { 
			{ To(s.position.x), To(s.position.y) }, 
			{ To(s.velocity.x), To(s.velocity.y) } 
		};
	}

	// Comparing both models' current state's position at the same:
	// - Time
	// - Steps
	// - Step Size (time interval h)
	template<typename T>
	double MaxRelativeDeviation(double mu, const Scenario& scenario, double tProp, int stepsPerLocalPeriod)
	{
		const Pcr3bp<T> pcrModel{ static_cast<T>(mu) };
		const TwoBody<T> tbModel{ static_cast<T>(1.0 - mu) }; // 1 - mu is the Earth's grav param

		// T_loc = 2pi * r0^1.5 / sqrt(1−mu)
		const double tLocal{ 2.0 * std::numbers::pi * std::pow(scenario.r0, 1.5) / std::sqrt(1.0 - mu) };
		// Steps n = ceil(tProp / (T_loc / N))
		const long long n{ static_cast<long long>(std::ceil(tProp / (tLocal / stepsPerLocalPeriod))) };
		// Step size h = tProp / n (time period)
		const T h{ static_cast<T>(tProp / n) };
		const double hDouble{ static_cast<double>(h) }; // Real time period in physics

		State<T> statePcr{ CastState<T>(Pcr3bpStart(scenario, mu)) };
		State<T> stateTb{ CastState<T>(TwoBodyStart(scenario, mu)) };

		double maxDeviation{ 0.0 };
		// steps?
		for (long long i{ 1 }; i <= n; ++i)
		{
			statePcr = pcrModel.Rk4Step(statePcr, h);
			stateTb = tbModel.Rk4Step(stateTb, h);

			const State<double> converted{ RotatingToEathCentered(CastState<double>(statePcr), mu, i * hDouble) };
			const State<double> two2{ CastState<double>(stateTb) };
			maxDeviation = std::max(maxDeviation, glm::length(converted.position - two2.position));
		}
		
		return maxDeviation / scenario.r0;
	}
}
