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
		static glm::vec4 Rk4(const glm::vec4& y, float h, Deriv&& f)
		{
			const glm::vec4 k1{ f(y) }; // beginning
			const glm::vec4 k2{ f(y + (h / 2) * k1) }; // midpoint, using k1
			const glm::vec4 k3{ f(y + (h / 2) * k2) }; // midpoint, using k2
			const glm::vec4 k4{ f(y + h * k3) }; // end, suing k3

			const glm::vec4 newY{ y + (h / 6.f) * (k1 + 2.f * k2 + 2.f * k3 + k4) };

			return newY;
		}

		// Kepler's third law: T = 2pi * sqrt(a^3 / mu)
		// a is the simi-major axis which is the radius for a circular orbit
		inline float OrbitPeriod(float semiMajorAxis, float mu) noexcept
		{
			return 2.f * static_cast<float>( std::numbers::pi ) * std::sqrt(semiMajorAxis * semiMajorAxis * semiMajorAxis / mu);
		}

		// The radius for circular orbit
		inline float SemiMajorAxis(const glm::vec4& s, float mu) noexcept
		{
			const glm::vec2 pos{ s.x, s.y };
			const glm::vec2 vel{ s.z, s.w };
			const float r{ glm::length(pos) };
			const float v2{ glm::dot(vel, vel) };
			const float invA{ 2.f / r - v2 / mu };

			return 1.f / invA;
		}
	}
}