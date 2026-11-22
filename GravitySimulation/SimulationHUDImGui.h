#pragma once
#include "ImGuiScene.h"
#include "Tests/IntegratorTest.h"
#include "Tests/CheckpointTest.h"
#include "Tests/PrecisionComparisonTest.h"

#include <vector>
#include <numbers>

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
		std::vector<ConvergenceRow> m_DoubleRows;
		std::vector<ConvergenceRow> m_FloatRows;
	};

	class SimulationHUDImGui2 final : public ge::ImGuiInstance
	{
	public:
		SimulationHUDImGui2(GravitySimulationApp* app);
		~SimulationHUDImGui2() override = default;

		void UpdateImGui(float) override {}
		void RenderImGui() override;

	private:
		GravitySimulationApp* m_App; // Cached ref

		double m_TestMu;
		std::vector<CheckResult> m_CheckpointRows;
	};

	class SimulationHUDImGui3 final : public ge::ImGuiInstance
	{
	public:
		SimulationHUDImGui3(GravitySimulationApp* app);
		~SimulationHUDImGui3() override = default;

		void UpdateImGui(float) override {}
		void RenderImGui() override;

	private:
		GravitySimulationApp* m_App; // Cached ref

		double m_TestMu;
		double m_TestPhase{ 0.0 };
		double m_TestTProp{ 2 * std::numbers::pi };
		int m_TestStepsPerLocal{ 200 };

		std::vector<PrecisionRow> m_PrecisionRows;
	};
}