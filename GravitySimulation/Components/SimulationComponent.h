#pragma once
#include "Components/Component.h"
#include "PhysicsCore.h"

#include <cstdint>

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
		SimulationComponent(ge::GameObject* owner);

		// Used only for rendering purposes for Transform double -> float conversion
		virtual void UpdateComponent(float deltaTime) override;
		// Physics-based time intervals
		virtual void FixedUpdateComponent(float engineDT) override;

	private:
		State m_CurrentState{};
		const double m_H{}; // Constant time interval
		const uint64_t m_MaxSubsteps{ 4 };
		const double m_TimeScale{ 0.1 };

		double m_Accumulator{ 0.0 };
		uint64_t m_StepCount{ 0 };
		BaseModel m_Model{ TwoBody(0) };

		static constexpr double WorldPerSimUnit{ 200.0 };  // 1 normalized distance = 200 world units at zoom 1
	};
}