#include "TrailComponent.h"
#include "SimulationConstants.h"

#include "Renderer.h"

gravity::TrailComponent::TrailComponent(ge::GameObject* owner, size_t maxPoints, double minSpacing, SDL_Color color)
	:Component::Component(owner),
	m_MaxPoints{ maxPoints },
	m_MinSpacing{ minSpacing },
	m_Color{ color }
{
}

void gravity::TrailComponent::RenderComponent() const
{
	if (m_Points.size() < 2) return;

	auto& renderer{ ge::Renderer::GetInstance() };
	m_ScreenPoints.clear();

	for (const glm::dvec2& p : m_Points)
	{
		const glm::vec2 s{ renderer.WorldToScreen({
			static_cast<float>(p.x * constants::WorldPerSimUnit),
			static_cast<float>(-p.y * constants::WorldPerSimUnit)}) }; // Invert y
		m_ScreenPoints.push_back({ s.x, s.y });
	}

	SDL_Renderer* sdl{ renderer.GetSDLRenderer() };
	SDL_SetRenderDrawColor(sdl, m_Color.r, m_Color.g, m_Color.b, m_Color.a);
	SDL_RenderLines(sdl, m_ScreenPoints.data(), static_cast<int>(m_ScreenPoints.size()));
	
}

void gravity::TrailComponent::AddPoint(const glm::dvec2& p)
{
	if (!m_Points.empty() && glm::distance(p, m_Points.back()) < m_MinSpacing)
		return;

	m_Points.push_back(p);

	if (m_Points.size() > m_MaxPoints)
		m_Points.pop_front(); // Pop the oldest point
}
