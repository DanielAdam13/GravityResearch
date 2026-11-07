#pragma once
#include "ImGuiScene.h"
#include "IntegratorTest.h"

#include <vector>

namespace gravity
{
	class GravitySimulationApp;

	class SimulationHUDImGui final : public ge::ImGuiInstance
	{
	public:
		SimulationHUDImGui(GravitySimulationApp* app);
		~SimulationHUDImGui() override = default;

		void UpdateImGui(float) override {}
		void RenderImGui() override;

	private:
		GravitySimulationApp* m_App; // Cached ref

		float m_TestMu{ 1.f };
		float m_TestRadius{ 1.f };
		std::vector<ConvergenceRow> m_Rows;
	};
}