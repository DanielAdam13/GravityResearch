#pragma once
#include <glm/glm.hpp>
#include <numbers>
#include <cmath>

namespace gravity
{
	template <typename T>
	using Vec2 = glm::vec<2, T>;
	template <typename T>
	using Vec4 = glm::vec<4, T>;

	namespace helpers
	{
		// The Rk4 formula:
		// input y -> current state 
		// output y' -> new state
		template <typename T, typename Deriv>
		Vec4<T> Rk4(const Vec4<T>& y, T h, Deriv&& f)
		{
			const T half{ T(0.5) };

			const Vec4<T> k1{ f(y) }; // beginning
			const Vec4<T> k2{ f(y + (half * h) * k1) }; // midpoint, using k1
			const Vec4<T> k3{ f(y + (half * h) * k2) }; // midpoint, using k2
			const Vec4<T> k4{ f(y + h * k3) }; // end, suing k3

			const Vec4<T> newY{ y + (h / T(6)) * (k1 + T(2) * k2 + T(2) * k3 + k4) };

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

		// Conserved energy. E = v^2 / 2 - mu / r
		inline double SpecificEnergy(const glm::dvec4& s, double mu) noexcept
		{
			const glm::dvec2 pos{ s.x, s.y };
			const glm::dvec2 vel{ s.z, s.w };

			return 0.5 * glm::dot(vel, vel) - mu / glm::length(pos);
		}

		// Conserved energy of a three-body system.
		inline double Jacobi(const glm::dvec4& s, double massRatio) noexcept
		{
			const glm::dvec2 vel{ s.z, s.w };
			const double posX{ s.x };
			const double posY{ s.y };

			// Specific convention
			const double r1{ std::sqrt((posX + massRatio) * (posX + massRatio) + posY * posY) };
			const double r2{ std::sqrt((posX - 1.0 + massRatio) * (posX - 1.0 + massRatio) + posY * posY) };

			return posX * posX + posY * posY + 2.0 * (1.0 - massRatio) / r1 + 2.0 * massRatio / r2 -
				glm::dot(vel, vel);
		}
	}
}