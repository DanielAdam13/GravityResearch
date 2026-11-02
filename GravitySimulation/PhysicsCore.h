#pragma once
#include "PhysicsHelpers.h"

#include <glm/glm.hpp>
#include <cmath>

namespace gravity
{
	struct State 
	{ 
		glm::dvec2 position; glm::dvec2 velocity;
	};

	class BaseModel
	{
	public:
		BaseModel()
		{}

		virtual State Rk4Step(const State& s, double h) const = 0;

	protected:

	private:

	};

	class TwoBody final : public BaseModel
	{
	public:
		explicit TwoBody(double mu) // Takes gravitational prameter -> mu = G * M
			:BaseModel(),
			m_Mu{ mu }
		{}

		virtual State Rk4Step(const State& s, double h) const override;

	private:
		double m_Mu; // Gravitational Parameter

		// The TwoBody version of the equiations of motion function. 
		// Derivative function with input: y = (x,y,vx,vy) and output: y' = (vx, vy, ax, ay)
		// This returns the "rate of change of y".
		inline glm::dvec4 F(const glm::dvec4& s) const
		{
			const double r2{ s.x * s.x + s.y * s.y };
			const double r3{ r2 * std::sqrt(r2) }; // r^3
			return { s.z, s.w, -m_Mu * s.x / r3, -m_Mu * s.y / r3 };
		}
	};

	class Pcr3bp final : public BaseModel
	{
	public:
		explicit Pcr3bp(double mu) // Secondary body’s share of the total mass: mu = m2 / (m1 + m2)
			:BaseModel(),
			m_Mu{ mu }
		{}

		virtual State Rk4Step(const State& s, double h) const override; // RK4

	private:
		double m_Mu; // second body's share of mass

		// The PCR3BP version of the equiations of motion function.
		// Derivative function with input: y = (x,y,vx,vy) and output: y' = (vx, vy, ax, ay)
		// This returns the "rate of change of y".
		inline glm::dvec4 F(const glm::dvec4& /**/) const
		{
			/*const double x{ s.x };
			const double y{ s.y };
			const double vx{ s.z };
			const double vy{ s.w };*/

			return {};
		}

	};
}