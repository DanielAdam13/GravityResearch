#pragma once
#include <glm/glm.hpp>
#include <numbers>
#include <cmath>

namespace gravity
{
	namespace helpers
	{
		// The Rk4 formula:
		// input y -> current state 
		// output y' -> new state
		template <typename Deriv>
		static glm::dvec4 Rk4(const glm::dvec4& y, double h, Deriv&& f)
		{
			const glm::dvec4 k1{ f(y) }; // beginning
			const glm::dvec4 k2{ f(y + (h / 2) * k1) }; // midpoint, using k1
			const glm::dvec4 k3{ f(y + (h / 2) * k2) }; // midpoint, using k2
			const glm::dvec4 k4{ f(y + h * k3) }; // end, suing k3

			const glm::dvec4 newY{ y + (h / 6.0) * (k1 + 2.0 * k2 + 2.0 * k3 + k4) };

			return newY;
		}

		// Kepler's third law: T = 2pi * sqrt(a^3 / mu)
		// a is the simi-major axis which is the radius for a circular orbit
		inline double OrbitPeriod(double semiMajorAxis, double mu) noexcept
		{
			return 2.0 * std::numbers::pi * std::sqrt(semiMajorAxis * semiMajorAxis * semiMajorAxis / mu);
		}

		// The radius for circular orbit
		inline double SemiMajorAxis(const glm::dvec4& s, double mu) noexcept
		{
			const glm::dvec2 pos{ s.x, s.y };
			const glm::dvec2 vel{ s.z, s.w };
			const double r{ glm::length(pos) };
			const double v2{ glm::dot(vel, vel) };
			const double invA{ 2.0 / r - v2 / mu };

			return 1.0 / invA;
		}
	}
}