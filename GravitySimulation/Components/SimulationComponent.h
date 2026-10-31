#pragma once
#include "Components/Component.h"

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

	};
}