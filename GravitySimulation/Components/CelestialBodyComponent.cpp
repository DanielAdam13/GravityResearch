#include "CelestialBodyComponent.h"
#include "GameObject.h"
#include "Components/Transform.h"
#include "SimulationConstants.h"
#include "Components/TrailComponent.h"
#include "Components/Image.h"

#include <algorithm>

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
		static_cast<float>(m_CurrentState.position.x * constants::WorldPerSimUnit),
		static_cast<float>(-m_CurrentState.position.y * constants::WorldPerSimUnit));
}

void gravity::CelestialBodyComponent::Reset() noexcept
{
	m_CurrentState = m_InitialState;
	if (auto* trail{ GetOwner()->GetComponent<TrailComponent>() })
	{
		trail->Clear();
	}
}

void gravity::CelestialBodyComponent::FitSpriteToRadius(double exaggeration, float minDiameterWorld)
{
	auto* image{ GetOwner()->GetComponent<ge::Image>() };
	if (!image || !image->GetTexture())
		return;

	const float diamater{ std::max(
		static_cast<float>(2.0 * m_Data.radius * constants::WorldPerSimUnit * exaggeration), minDiameterWorld) };
	const float scale{ diamater / image->GetTexture()->GetSize().x }; // Keeps aspect ratio

	GetOwner()->GetComponent<ge::Transform>()->SetLocalScale(scale, scale, 1.f);
}

void gravity::CelestialBodyComponent::SetState(const State& state) noexcept
{
	m_CurrentState = state;
	if (auto* trail{ GetOwner()->GetComponent<TrailComponent>() })
	{
		trail->AddPoint(state.position);
	}
}
