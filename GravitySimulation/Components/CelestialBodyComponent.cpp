#include "CelestialBodyComponent.h"
#include "GameObject.h"
#include "Components/Transform.h"

gravity::CelestialBodyComponent::CelestialBodyComponent(ge::GameObject* owner, BodyData data, const State& initialState)
	:ge::Component::Component(owner),
	m_Data{ data },
	m_InitialState{ initialState },
	m_CurrentState{ initialState }
{
}

void gravity::CelestialBodyComponent::UpdateComponent(float)
{
	GetOwner()->GetComponent<ge::Transform>()->SetLocalPosition(
		static_cast<float>(m_CurrentState.position.x * WorldPerSimUnit),
		static_cast<float>(-m_CurrentState.position.y * WorldPerSimUnit));
}
