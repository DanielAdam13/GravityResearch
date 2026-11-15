#pragma once
#include "PhysicsHelpers.h"

#include <glm/glm.hpp>
#include <cmath>
#include <string>

namespace gravity
{
	struct State 
	{ 
		glm::dvec2 position; 
		glm::dvec2 velocity;
	};

	struct BodyData
	{
		std::string name;
		double mu; // Gravitational constant -> GM
		double radius; // Physical radius
	};

	class BaseModel
	{
	public:
		BaseModel() = default;

		virtual ~BaseModel() = default;

		virtual State Rk4Step(const State& s, double h) const = 0;

	protected:

	private:

	};

	class TwoBody final : public BaseModel
	{
	public:
		explicit TwoBody(double mu, const glm::dvec2& centralPosition = { 0.0, 0.0 }) // Takes gravitational prameter -> mu = G * M
			:BaseModel(),
			m_Mu{ mu },
			m_CentralPosition{ centralPosition }
		{}

		virtual State Rk4Step(const State& s, double h) const override;

		// When the central body moves between steps
		void SetCentralPosition(const glm::dvec2& p) noexcept { m_CentralPosition = p; }

	private:
		double m_Mu; // Gravitational Parameter
		glm::dvec2 m_CentralPosition; // Where the attracting body is

		/* 
		 * The TwoBody version of the equiations of motion function.
		 * Derivative function with input: y = (x,y,vx,vy) and output: y' = (vx, vy, ax, ay)
		 * This returns the "rate of change of y".
		 */
		inline glm::dvec4 F(const glm::dvec4& s) const
		{
			const double dx{ s.x - m_CentralPosition.x };   // vector from central body to satellite
			const double dy{ s.y - m_CentralPosition.y };
			const double r2{ dx * dx + dy * dy };
			const double r3{ r2 * std::sqrt(r2) }; // r^3
			return { s.z, s.w, -m_Mu * dx / r3, -m_Mu * dy / r3 };
		}
	};

	class Pcr3bp final : public BaseModel
	{
	public:
		explicit Pcr3bp(double mu) // Secondary body’s share of the total mass: mu = m2 / (m1 + m2)
			:BaseModel(),
			m_MassRatio{ mu }
		{}

		virtual State Rk4Step(const State& s, double h) const override; // RK4

	private:
		double m_MassRatio; // second body's share of mass

		/*
		 * The PCR3BP version of the equiations of motion function.
		 * Derivative function with input: y = (x,y,vx,vy) and output: y' = (vx, vy, ax, ay)
		 * This returns the "rate of change of y".
		 */
		// 
		inline glm::dvec4 F(const glm::dvec4& s) const
		{
			const double x{ s.x };
			const double y{ s.y };
			const double vx{ s.z };
			const double vy{ s.w };

			const double dx1{ x + m_MassRatio }; // x-distance from the big primary
			const double dx2{ x - 1.0 + m_MassRatio }; // x-distance from the small primary

			const double r1Cubed{ std::pow(dx1 * dx1 + y * y, 1.5) };
			const double r2Cubed{ std::pow(dx2 * dx2 + y * y, 1.5) };

			// The +x and +y are the centrifugal force
			const double ax{ 2.0 * vy + x - (1.0 - m_MassRatio) * dx1 / r1Cubed - m_MassRatio * dx2 / r2Cubed }; // 2*vy -> Coriolis effect
			const double ay{ -2.0 * vx + y - (1.0 - m_MassRatio) * y / r1Cubed - m_MassRatio * y / r2Cubed }; // -2*vx -> Coriolis effect

			return { vx, vy, ax, ay };
		}

	};
}