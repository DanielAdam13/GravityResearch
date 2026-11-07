#include "PhysicsCore.h"

gravity::State gravity::TwoBody::Rk4Step(const State& s, float h) const
{
	// Intitial state
	const glm::vec4 y{ s.position, s.velocity }; // (x, y, vx, vy)

	// RK4
	const glm::vec4 newY{ helpers::Rk4(y, h, [this](const glm::vec4& q) -> glm::vec4
		{
			return TwoBody::F(q);
		}) };

	// New state
	return State{
		glm::vec2{ newY.x, newY.y },
		glm::vec2{ newY.z, newY.w } };
}

gravity::State gravity::Pcr3bp::Rk4Step(const State& s, float h) const
{
	// Intitial state
	const glm::vec4 y{ s.position, s.velocity }; // (x, y, vx, vy)

	// RK4
	glm::vec4 newY{ helpers::Rk4(y, h, [this](const glm::vec4& q) -> glm::vec4
		{
			return Pcr3bp::F(q);
		}) };

	// New state
	return State{
		glm::vec2{ newY.x, newY.y },
		glm::vec2{ newY.z, newY.w } };
}
