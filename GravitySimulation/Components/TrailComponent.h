#pragma once
#include "Components/Component.h"

#include <deque>
#include "glm/glm.hpp"
#include <SDL3/SDL.h>
#include <vector>

namespace gravity
{
	/*
	* Signifies a Celestial Body - Sun, planets, moons, satellite.
	* Owns the immutable BodyData.
	* Manages the current state of an object.
	*/
	class TrailComponent final : public ge::Component
	{
	public:
		// ---- TYPE IDENTIFIER ----
		// Every Train Instance shares the same component type ID
		static constexpr ge::ComponentTypeID StaticTypeID{ 2 };

		TrailComponent(ge::GameObject* owner, size_t maxPoints = 5000,
			double minSpacing = 0.02, SDL_Color color = { 255, 200, 0, 255 });

		virtual void UpdateComponent(float) override {}
		virtual void FixedUpdateComponent(float) override {}
		virtual void RenderComponent() const override;

		// Called by CelestialBodyComponent::SetState
		void AddPoint(const glm::dvec2& p);
		void Clear() { m_Points.clear(); }

	private:
		std::deque<glm::dvec2> m_Points; // Deque so pop/push_front, pop/push_back
		mutable std::vector<SDL_FPoint> m_ScreenPoints; // Used by SDL_Renderer

		size_t m_MaxPoints;
		double m_MinSpacing; // Sim units (0.02 = 4 px at zoom)
		SDL_Color m_Color;

	};
}