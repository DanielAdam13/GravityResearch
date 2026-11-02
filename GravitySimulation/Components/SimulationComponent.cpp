#include "SimulationComponent.h"
#include "GameObject.h"

gravity::SimulationComponent::SimulationComponent(ge::GameObject* owner)
	:ge::Component::Component(owner)
{
}

void gravity::SimulationComponent::UpdateComponent(float /*engineDT*/)
{
	GetOwner()->GetComponent<ge::Transform>()->SetLocalPosition(
		static_cast<float>(m_CurrentState.position.x * WorldPerSimUnit),
		static_cast<float>(-m_CurrentState.position.y * WorldPerSimUnit) // Invert SDL's y down axis
	);
}

void gravity::SimulationComponent::FixedUpdateComponent(float engineDT)
{
	m_Accumulator += m_TimeScale * static_cast<double>(engineDT); // Simulation units per real second
	int steps{ 0 };
	while (m_Accumulator >= m_H && steps < m_MaxSubsteps)
	{
		m_CurrentState = m_Model.Rk4Step(m_CurrentState, m_H);
		++m_StepCount; // time = count * h
		m_Accumulator -= m_H;
		++steps;
	}

	if (steps == m_MaxSubsteps)
	{
		m_Accumulator = 0.0; // reset accumulator, avoid a runaway loop
	}
}
