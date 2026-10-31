#include "PhysicsCore.h"

gravity::State gravity::TwoBody::Rk4Step(const State& s, double h) const
{
	// Intitial state
	const glm::dvec4 y{ s.position, s.velocity }; // (x, y, vx, vy)

	// RK4
	const glm::dvec4 newY{ helpers::Rk4(y, h, [this](const glm::dvec4& q) -> glm::vec4
		{
			return TwoBody::F(q);
		}) };

	// New state
	return State{
		glm::dvec2{ newY.x, newY.y },
		glm::dvec2{ newY.z, newY.w } };
}

gravity::State gravity::Pcr3bp::Rk4Step(const State& s, double h) const
{
	// Intitial state
	const glm::dvec4 y{ s.position, s.velocity }; // (x, y, vx, vy)

	// RK4
	glm::dvec4 newY{ helpers::Rk4(y, h, [this](const glm::dvec4& q) -> glm::vec4
		{
			return Pcr3bp::F(q);
		}) };

	// New state
	return State{
		glm::dvec2{ newY.x, newY.y },
		glm::dvec2{ newY.z, newY.w } };
}
