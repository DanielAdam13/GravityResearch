#pragma once
#include "Components/Component.h"
#include "PhysicsCore.h"

#include <cstdint>
#include <memory>

namespace ge
{
	class GameObject;
}

namespace gravity
{
	// OWNS the current state of the set MODEL and sets local position
	class SimulationComponent final : public ge::Component
	{
	public:
		// ---- TYPE IDENTIFIER ----
		// Every Anomator Instance shares the same component type ID
		static constexpr ge::ComponentTypeID StaticTypeID{ 0 };

		SimulationComponent(ge::GameObject* owner, std::unique_ptr<BaseModel> model, const State& initialState, float timeIntervalH = 0.001f);

		// Used only for rendering purposes for Transform double -> float conversion
		virtual void UpdateComponent(float deltaTime) override;
		// Physics-based time intervals
		virtual void FixedUpdateComponent(float engineDT) override;
		virtual void RenderComponent() const override {}

	private:
		std::unique_ptr<BaseModel> m_Model;

		State m_CurrentState;
		const State m_InitialState; // To reset to from ImGui

		const float m_H; // Constant time interval
		static constexpr uint64_t m_MaxSubsteps{ 1000 };
		const float m_TimeScale; // Smoother frames for visual simulation - DOES NOT AFFECT PHYSICS, only UpdateComponent's position setting
		float m_Accumulator{ 0.f };
		uint64_t m_StepCount{ 0 };

		static constexpr float WorldPerSimUnit{ 200.f };  // 1 normalized distance = 200 world units at zoom 1
	};
}