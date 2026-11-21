#include "PhysicsCore.h"

gravity::State gravity::TwoBody::Rk4Step(const State& s, double h) const
{
	// Intitial state
	const glm::dvec4 y{ s.position, s.velocity }; // (x, y, vx, vy)

	// RK4
	const glm::dvec4 newY{ helpers::Rk4(y, h, [this](const glm::dvec4& q) -> glm::dvec4
		{
			return TwoBody::F(q);
		}) };

	// New state
	return State{
		glm::dvec2{ newY.x, newY.y },
		glm::dvec2{ newY.z, newY.w } };
}

double gravity::TwoBody::ConservedQuantity(const State& s) const
{
	return helpers::SpecificEnergy({s.position, s.velocity}, m_Mu);
}

glm::dvec4 gravity::TwoBody::F(const glm::dvec4& s) const
{
	const double dx{ s.x - m_CentralPosition.x };   // vector from central body to satellite
	const double dy{ s.y - m_CentralPosition.y };
	const double r2{ dx * dx + dy * dy };
	const double r3{ r2 * std::sqrt(r2) }; // r^3
	return { s.z, s.w, -m_Mu * dx / r3, -m_Mu * dy / r3 };
}

gravity::State gravity::Pcr3bp::Rk4Step(const State& s, double h) const
{
	// Intitial state
	const glm::dvec4 y{ s.position, s.velocity }; // (x, y, vx, vy)

	// RK4
	glm::dvec4 newY{ helpers::Rk4(y, h, [this](const glm::dvec4& q) -> glm::dvec4
		{
			return Pcr3bp::F(q);
		}) };

	// New state
	return State{
		glm::dvec2{ newY.x, newY.y },
		glm::dvec2{ newY.z, newY.w } };
}

double gravity::Pcr3bp::ConservedQuantity(const State& s) const
{
	return helpers::Jacobi({ s.position, s.velocity }, m_MassRatio);
}

glm::dvec4 gravity::Pcr3bp::F(const glm::dvec4& s) const
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
