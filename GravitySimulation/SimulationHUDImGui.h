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

		double m_TestMu{ 1.0 };
		double m_TestRadius{ 1.0 };
		std::vector<ConvergenceRow> m_Rows;
	};
}