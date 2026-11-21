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
		virtual double ConservedQuantity(const State& s) const = 0;

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
		/*
		 * The TwoBody's conserved quantity.
		 * Returns the Energy since the momentum and energy are conserved in a two-body system.
		 * E = v^2 / 2 - mu / r
		 */
		virtual double ConservedQuantity(const State& s) const override;

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
		glm::dvec4 F(const glm::dvec4& s) const;

	};

	class Pcr3bp final : public BaseModel
	{
	public:
		explicit Pcr3bp(double mu) // Secondary body’s share of the total mass: mu = m2 / (m1 + m2)
			:BaseModel(),
			m_MassRatio{ mu }
		{}

		virtual State Rk4Step(const State& s, double h) const override; // RK4
		virtual double ConservedQuantity(const State& s) const override;

	private:
		double m_MassRatio; // second body's share of mass

		/*
		 * The PCR3BP version of the equiations of motion function.
		 * Derivative function with input: y = (x,y,vx,vy) and output: y' = (vx, vy, ax, ay)
		 * This returns the "rate of change of y".
		 */
		// 
		glm::dvec4 F(const glm::dvec4& s) const;

	};
}