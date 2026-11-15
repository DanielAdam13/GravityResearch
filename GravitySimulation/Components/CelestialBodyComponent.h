#pragma once
#include "Components/Component.h"

#include "PhysicsCore.h"

namespace gravity
{
	/*
	* Signifies a Celestial Body - Sun, planets, moons, satellite.
	* Owns the immutable BodyData.
	* Manages the current state of an object.
	*/
	class CelestialBodyComponent final : public ge::Component
	{
	public:
		// ---- TYPE IDENTIFIER ----
		// Every Celestial Body Instance shares the same component type ID
		static constexpr ge::ComponentTypeID StaticTypeID{ 1 };

		CelestialBodyComponent(ge::GameObject* owner, BodyData data, const State& initialState);

		// Used only for rendering purposes for Transform double -> float conversion
		virtual void UpdateComponent(float) override;
		virtual void FixedUpdateComponent(float) override {}
		virtual void RenderComponent() const override {}

		const BodyData& GetBodyData() const noexcept { return m_Data; }
		const State& GetCurrentState() const noexcept { return m_CurrentState; }
		void Reset() noexcept;

		// Called directly from the central SimulationComponent from the accumulator logic.
		// Also calls AddPoint to the Trail if not nullptr
		void SetState(const State& state) noexcept;

	private:
		const BodyData m_Data; // Immutable data storing Celestial Body's name, mu, radius
		const State m_InitialState; // For Reset()
		State m_CurrentState;
	};
}