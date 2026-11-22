#include "SimulationComponent.h"
#include "GameObject.h"

#include "Components/CelestialBodyComponent.h"

#include <memory>

gravity::SimulationComponent::SimulationComponent(ge::GameObject* owner, std::unique_ptr<BaseModel<double>> model, CelestialBodyComponent* orbiter,
	double timeIntervalH)
	:ge::Component::Component(owner),
	m_Model{ std::move(model) },
	m_CachedMu{ m_Model->GetMu() },
	/*m_Central{central},*/
	m_Orbiter{ orbiter },
	m_H{ timeIntervalH },
	m_TimeScale{ m_H * 60 }
{
}

void gravity::SimulationComponent::FixedUpdateComponent(float engineDT)
{
	m_Accumulator += m_TimeScale * static_cast<double>(engineDT); // Simulation units per real second
	uint64_t steps{ 0 };
	while (m_Accumulator >= m_H && steps < m_MaxSubsteps)
	{
		m_Orbiter->SetState(m_Model->Rk4Step(m_Orbiter->GetCurrentState(), m_H));
		++m_StepCount; // time = count * h
		m_Accumulator -= m_H;
		++steps;
	}

	if (steps == m_MaxSubsteps)
	{
		m_Accumulator = 0.0; // reset accumulator, avoid a runaway loop
	}
}

void gravity::SimulationComponent::ResetModel(std::unique_ptr<BaseModel<double>> model)
{
	m_Model = std::move(model);
	Reset();
}

void gravity::SimulationComponent::Reset()
{
	//m_Central->Reset();
	m_Orbiter->Reset();
	m_Accumulator = 0.0;
	m_StepCount = 0;
}

double gravity::SimulationComponent::GetModelConservedQuantity() const
{
	return m_Model->ConservedQuantity(m_Orbiter->GetCurrentState());
}

double gravity::SimulationComponent::GetModelCachedMu() const
{
	return m_Model->GetMu();
}
