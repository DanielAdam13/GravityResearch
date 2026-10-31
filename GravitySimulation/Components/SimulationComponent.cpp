#include "SimulationComponent.h"

gravity::SimulationComponent::SimulationComponent(ge::GameObject* owner)
	:ge::Component::Component(owner)
{
}

void gravity::SimulationComponent::UpdateComponent(float /*deltaTime*/)
{
}

void gravity::SimulationComponent::FixedUpdateComponent(float /*engineDT*/)
{
}
