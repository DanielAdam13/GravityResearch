#pragma once
#include "PhysicsHelpers.h"

#include <glm/glm.hpp>
#include <cmath>
#include <string>

namespace gravity
{
	template <typename T>
	struct State
	{
		Vec2<T> position; 
		Vec2<T> velocity;
	};

	struct BodyData
	{
		std::string name;
		double mu; // Gravitational constant -> GM
		double radius; // Physical radius
	};

	// Measurements (energy, Jacobi) are always computed in double
	template <typename T>
	inline glm::dvec4 ToDouble(const State<T>& s) noexcept
	{
		return { static_cast<double>(s.position.x), static_cast<double>(s.position.y),
				 static_cast<double>(s.velocity.x), static_cast<double>(s.velocity.y) };
	}

	template <typename T>
	class BaseModel
	{
	public:
		BaseModel() = default;

		virtual ~BaseModel() = default;

		virtual State<T> Rk4Step(const State<T>& s, T h) const = 0;
		virtual double ConservedQuantity(const State<T>& s) const = 0;

		virtual double GetMu() const noexcept = 0;

	protected:

	private:

	};

	template <typename T>
	class TwoBody final : public BaseModel<T>
	{
	public:
		explicit TwoBody(T mu, const Vec2<T>& centralPosition = { T(0), T(0)}) // Takes gravitational prameter -> mu = G * M
			:BaseModel<T>(),
			m_Mu{ mu },
			m_CentralPosition{ centralPosition }
		{}

		virtual State<T> Rk4Step(const State<T>& s, T h) const override
		{
			// Intitial state
			const Vec4<T> y{ s.position, s.velocity }; // (x, y, vx, vy)

			// RK4
			const Vec4<T> newY{ helpers::Rk4<T>(y, h, [this](const Vec4<T>& q) -> Vec4<T>
				{
					return TwoBody::F(q);
				}) };

			// New state
			return { { newY.x, newY.y }, { newY.z, newY.w } };
		}
		/*
		 * The TwoBody's conserved quantity.
		 * Returns the Energy since the momentum and energy are conserved in a two-body system.
		 * E = v^2 / 2 - mu / r
		 */
		virtual double ConservedQuantity(const State<T>& s) const override
		{
			return helpers::SpecificEnergy( ToDouble(s), static_cast<double>(m_Mu));
		}
		virtual double GetMu() const noexcept override { return static_cast<double>(m_Mu); }

		// When the central body moves between steps
		void SetCentralPosition(const Vec2<T>& p) noexcept { m_CentralPosition = p; }

	private:
		T m_Mu; // Gravitational Parameter
		Vec2<T> m_CentralPosition; // Where the attracting body is

		/* 
		 * The TwoBody version of the equiations of motion function.
		 * Derivative function with input: y = (x,y,vx,vy) and output: y' = (vx, vy, ax, ay)
		 * This returns the "rate of change of y".
		 */
		Vec4<T> F(const Vec4<T>& s) const
		{
			const T dx{ s.x - m_CentralPosition.x };   // vector from central body to satellite
			const T dy{ s.y - m_CentralPosition.y };
			const T r2{ dx * dx + dy * dy };
			const T r3{ r2 * std::sqrt(r2) }; // r^3
			return { s.z, s.w, -m_Mu * dx / r3, -m_Mu * dy / r3 };
		}

	};

	template <typename T>
	class Pcr3bp final : public BaseModel<T>
	{
	public:
		explicit Pcr3bp(T massRatio) // Secondary body’s share of the total mass: mu = m2 / (m1 + m2)
			:BaseModel<T>(),
			m_MassRatio{ massRatio }
		{}

		virtual State<T> Rk4Step(const State<T>& s, T h) const override // RK4
		{
			// Intitial state
			const Vec4<T> y{ s.position, s.velocity }; // (x, y, vx, vy)

			// RK4
			Vec4<T> newY{ helpers::Rk4<T>(y, h, [this](const Vec4<T>& q) -> Vec4<T>
				{
					return Pcr3bp::F(q);
				}) };

			// New state
			return { { newY.x, newY.y }, { newY.z, newY.w } };
		}

		virtual double ConservedQuantity(const State<T>& s) const override
		{
			return helpers::Jacobi(ToDouble(s), static_cast<double>(m_MassRatio));
		}

		virtual double GetMu() const noexcept override { return static_cast<double>(m_MassRatio); }

	private:
		T m_MassRatio; // second body's share of mass

		/*
		 * The PCR3BP version of the equiations of motion function.
		 * Derivative function with input: y = (x,y,vx,vy) and output: y' = (vx, vy, ax, ay)
		 * This returns the "rate of change of y".
		 */
		// 
		Vec4<T> F(const Vec4<T>& s) const
		{
			const T x{ s.x };
			const T y{ s.y };
			const T vx{ s.z };
			const T vy{ s.w };

			const T dx1{ x + m_MassRatio }; // x-distance from the big primary
			const T dx2{ x - T(1) + m_MassRatio}; // x-distance from the small primary

			const T r1sq{ dx1 * dx1 + s.y * s.y };
			const T r2sq{ dx2 * dx2 + s.y * s.y };
			const T r1Cubed{ r1sq * std::sqrt(r1sq) };
			const T r2Cubed{ r1sq * std::sqrt(r2sq) };

			// The +x and +y are the centrifugal force
			const T ax{ T(2) * vy + x - (T(1) - m_MassRatio) * dx1 / r1Cubed - m_MassRatio * dx2 / r2Cubed }; // 2*vy -> Coriolis effect
			const T ay{ -T(2) * vx + y - (T(1) - m_MassRatio) * y / r1Cubed - m_MassRatio * y / r2Cubed }; // -2*vx -> Coriolis effect

			return { vx, vy, ax, ay };
		}

	};
}