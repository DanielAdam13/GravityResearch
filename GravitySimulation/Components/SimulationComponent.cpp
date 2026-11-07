#include "SimulationComponent.h"
#include "GameObject.h"

#include <utility>

gravity::SimulationComponent::SimulationComponent(ge::GameObject* owner, std::unique_ptr<BaseModel> model, const State& initialState, float timeIntervalH)
	:ge::Component::Component(owner),
	m_Model{ std::move(model) },
	m_CurrentState{ initialState },
	m_InitialState{ initialState },
	m_H{ timeIntervalH },
	m_TimeScale{ m_H * 60 }
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
	m_Accumulator += m_TimeScale * static_cast<float>(engineDT); // Simulation units per real second
	int steps{ 0 };
	while (m_Accumulator >= m_H && steps < m_MaxSubsteps)
	{
		m_CurrentState = m_Model->Rk4Step(m_CurrentState, m_H);
		++m_StepCount; // time = count * h
		m_Accumulator -= m_H;
		++steps;
	}

	if (steps == m_MaxSubsteps)
	{
		m_Accumulator = 0.f; // reset accumulator, avoid a runaway loop
	}
}
