#pragma once
#include "PhysicsHelpers.h"

#include <glm/glm.hpp>
#include <cmath>

namespace gravity
{
	struct State 
	{ 
		glm::vec2 position; 
		glm::vec2 velocity;
	};

	class BaseModel
	{
	public:
		BaseModel()
		{}

		virtual State Rk4Step(const State& s, float h) const = 0;

	protected:

	private:

	};

	class TwoBody final : public BaseModel
	{
	public:
		explicit TwoBody(float mu) // Takes gravitational prameter -> mu = G * M
			:BaseModel(),
			m_Mu{ mu }
		{}

		virtual State Rk4Step(const State& s, float h) const override;

	private:
		float m_Mu; // Gravitational Parameter

		// The TwoBody version of the equiations of motion function. 
		// Derivative function with input: y = (x,y,vx,vy) and output: y' = (vx, vy, ax, ay)
		// This returns the "rate of change of y".
		inline glm::vec4 F(const glm::vec4& s) const
		{
			const float r2{ s.x * s.x + s.y * s.y };
			const float r3{ r2 * std::sqrt(r2) }; // r^3
			return { s.z, s.w, -m_Mu * s.x / r3, -m_Mu * s.y / r3 };
		}
	};

	class Pcr3bp final : public BaseModel
	{
	public:
		explicit Pcr3bp(float mu) // Secondary body’s share of the total mass: mu = m2 / (m1 + m2)
			:BaseModel(),
			m_Mu{ mu }
		{}

		virtual State Rk4Step(const State& s, float h) const override; // RK4

	private:
		float m_Mu; // second body's share of mass

		// The PCR3BP version of the equiations of motion function.
		// Derivative function with input: y = (x,y,vx,vy) and output: y' = (vx, vy, ax, ay)
		// This returns the "rate of change of y".
		inline glm::vec4 F(const glm::vec4& /**/) const
		{
			/*const double x{ s.x };
			const double y{ s.y };
			const double vx{ s.z };
			const double vy{ s.w };*/

			return {};
		}

	};
}