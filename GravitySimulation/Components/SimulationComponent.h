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
	class CelestialBodyComponent;

	/*
	* Progresses the accumulation of the simulation.
	* Creates and Owns the model - TwoBody or PCR3BP.
	*/
	class SimulationComponent final : public ge::Component
	{
	public:
		// ---- TYPE IDENTIFIER ----
		// Every Simulation Component Instance shares the same component type ID
		static constexpr ge::ComponentTypeID StaticTypeID{ 0 };

		SimulationComponent(ge::GameObject* owner, CelestialBodyComponent* central, CelestialBodyComponent* orbiter,
			double timeIntervalH = 0.0001);

		virtual void UpdateComponent(float) override {}
		// Physics-based time intervals
		virtual void FixedUpdateComponent(float engineDT) override;
		virtual void RenderComponent() const override {}

		void Reset();

	private:
		std::unique_ptr<BaseModel> m_Model;

		CelestialBodyComponent* m_Central;
		CelestialBodyComponent* m_Orbiter;

		// Accumulation variables
		// ----------------------------------
		const double m_H; // Constant time interval
		static constexpr uint64_t m_MaxSubsteps{ 1000 };
		const double m_TimeScale; // Smoother frames for visual simulation - How many steps per real second?, only UpdateComponent's position setting
		double m_Accumulator{ 0.0 };
		uint64_t m_StepCount{ 0 };
		// ----------------------------------
	};
}