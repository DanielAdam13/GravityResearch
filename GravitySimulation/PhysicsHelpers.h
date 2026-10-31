#pragma once
#include <glm/glm.hpp>

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
	}
}