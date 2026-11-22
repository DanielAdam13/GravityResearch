#include "SimulationHUDImGui.h"
#include "GravitySimulationApp.h"

#include "SimulationConstants.h"

#include <imgui.h>
#include <array>

gravity::SimulationHUDImGui::SimulationHUDImGui(GravitySimulationApp* app)
	:m_App{ app }
{
}

void gravity::SimulationHUDImGui::RenderImGui()
{
	if (!m_App)
		return;

	ImGui::Begin("Double vs Float numerical errors");

	// -----------------------------------------------------------------
	// Visualization toggles
	// -----------------------------------------------------------------
	ImGui::Spacing();
	ImGui::TextUnformatted("Integrator test: steps per orbit (RK4, circular two-body orbit)");
	ImGui::Separator();

	ImGui::InputDouble("mu", &m_TestMu, 0.0, 0.0, "%.4f");
	ImGui::InputDouble("radius", &m_TestRadius, 0.0, 0.0, "%.4f");

	// Only on the button press
	if (ImGui::Button("Run test") && m_TestMu > 0.0 && m_TestRadius > 0.0)
	{
		m_DoubleRows = gravity::RunStepConvergence<double>(m_TestMu, m_TestRadius, { 16, 32, 64, 128, 256, 512, 1024 }, { 1, 10, 100 });
		m_FloatRows = gravity::RunStepConvergence<float>(m_TestMu, m_TestRadius, { 16, 32, 64, 128, 256, 512, 1024 }, { 1, 10, 100 });
	}

	if (!m_DoubleRows.empty() && !m_FloatRows.empty())
	{
		ImGui::TextUnformatted("DOUBLE");

		if (ImGui::BeginTable("convergenceD", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
		{
			ImGui::TableSetupColumn("Steps/orbit");
			ImGui::TableSetupColumn("Orbits");
			ImGui::TableSetupColumn("Radius error");
			ImGui::TableSetupColumn("Position error");
			ImGui::TableSetupColumn("Energy drift");
			ImGui::TableHeadersRow();

			for (const auto& row : m_DoubleRows)
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn(); ImGui::Text("%d", row.stepsPerOrbit);
				ImGui::TableNextColumn(); ImGui::Text("%d", row.orbits);
				ImGui::TableNextColumn(); ImGui::Text("%.2e", row.radiusError);
				ImGui::TableNextColumn(); ImGui::Text("%.2e", row.positionError);
				ImGui::TableNextColumn(); ImGui::Text("%.2e", row.energyDrift);
			}
			ImGui::EndTable();
		}

		ImGui::TextUnformatted("FLOAT");

		if (ImGui::BeginTable("convergenceF", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
		{
			ImGui::TableSetupColumn("Steps/orbit");
			ImGui::TableSetupColumn("Orbits");
			ImGui::TableSetupColumn("Radius error");
			ImGui::TableSetupColumn("Position error");
			ImGui::TableSetupColumn("Energy drift");
			ImGui::TableHeadersRow();

			for (const auto& row : m_FloatRows)
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn(); ImGui::Text("%d", row.stepsPerOrbit);
				ImGui::TableNextColumn(); ImGui::Text("%d", row.orbits);
				ImGui::TableNextColumn(); ImGui::Text("%.2e", row.radiusError);
				ImGui::TableNextColumn(); ImGui::Text("%.2e", row.positionError);
				ImGui::TableNextColumn(); ImGui::Text("%.2e", row.energyDrift);
			}
			ImGui::EndTable();
		}
	}


	ImGui::End();
}

gravity::SimulationHUDImGui2::SimulationHUDImGui2(GravitySimulationApp* app)
	:m_App{ app },
	m_TestMu{ physConstants::MoonMu / (physConstants::EarthMu + physConstants::MoonMu) }
{
}

void gravity::SimulationHUDImGui2::RenderImGui()
{
	if (!m_App)
		return;

	ImGui::Begin("Checkpoints");

	// -----------------------------------------------------------------
	// Visualization toggles
	// -----------------------------------------------------------------
	ImGui::Spacing();
	ImGui::TextUnformatted("Checkpoints test");
	ImGui::Separator();

	ImGui::InputDouble("mass ratio (Moon / (Moon + Earth))", &m_TestMu, 0.0, 0.0, "%.9f");

	// Only on the button press
	if (ImGui::Button("Run test") && m_TestMu > 0.0 && m_TestMu < 0.5)
	{
		m_CheckpointRows = RunCheckpoints(m_TestMu);
	}

	if (!m_CheckpointRows.empty())
	{
		if (ImGui::BeginTable("checkpoint_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
		{
			ImGui::TableSetupColumn("Name");
			ImGui::TableSetupColumn("Value");
			ImGui::TableSetupColumn("Limit");
			ImGui::TableSetupColumn("Pass");
			ImGui::TableHeadersRow();

			for (const auto& row : m_CheckpointRows)
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn(); ImGui::Text("%s", row.name.c_str());
				ImGui::TableNextColumn(); ImGui::Text("%.3e", row.value);
				ImGui::TableNextColumn(); ImGui::Text("%.3e", row.limit);
				ImGui::TableNextColumn(); ImGui::Text("%s", row.pass ? "PASS" : "FAIL");;
			}
			ImGui::EndTable();
		}
	}

	ImGui::End();
}

gravity::SimulationHUDImGui3::SimulationHUDImGui3(GravitySimulationApp* app)
	:m_App{ app },
	m_TestMu{ physConstants::MoonMu / (physConstants::EarthMu + physConstants::MoonMu) }
{
}

void gravity::SimulationHUDImGui3::RenderImGui()
{
	if (!m_App)
		return;

	ImGui::Begin("Precision test");

	// -----------------------------------------------------------------
	// Visualization toggles
	// -----------------------------------------------------------------
	ImGui::Spacing();
	ImGui::TextUnformatted("Double vs Float precision test");
	ImGui::Separator();

	ImGui::InputDouble("mass ratio", &m_TestMu, 0.0, 0.0, "%.9f");
	ImGui::InputDouble("phase", &m_TestPhase, 0.0, 0.0, "%.3f");
	ImGui::InputInt("steps per local period", &m_TestStepsPerLocal, 0, 0);
	m_TestStepsPerLocal = std::max(m_TestStepsPerLocal, 10);

	std::vector<double> radii{ 0.02, 0.05, 0.06, 0.095, 0.15, 0.186, 0.364 };

	// Only on the button press
	if (ImGui::Button("Run test") && m_TestMu > 0.0)
	{
		m_PrecisionRows = RunPrecisionComparison(m_TestMu, m_TestPhase, m_TestTProp, m_TestStepsPerLocal, radii);
	}

	if (!m_PrecisionRows.empty())
	{
		if (ImGui::BeginTable("precision_table", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
		{
			ImGui::TableSetupColumn("r0/Start Radius");
			ImGui::TableSetupColumn("Double deviation");
			ImGui::TableSetupColumn("Float deviation");
			ImGui::TableHeadersRow();

			for (const auto& row : m_PrecisionRows)
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn(); ImGui::Text("%.2e", row.r0);
				ImGui::TableNextColumn(); ImGui::Text("%.2e", row.devDouble);
				ImGui::TableNextColumn(); ImGui::Text("%.2e", row.devFloat);

			}
			ImGui::EndTable();
		}
	}

	ImGui::End();
}
