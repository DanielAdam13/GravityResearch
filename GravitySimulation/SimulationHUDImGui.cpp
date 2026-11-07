#include "SimulationHUDImGui.h"
#include "GravitySimulationApp.h"

#include <imgui.h>

gravity::SimulationHUDImGui::SimulationHUDImGui(GravitySimulationApp* app)
	:m_App{ app }
{
}

void gravity::SimulationHUDImGui::RenderImGui()
{
	if (!m_App)
		return;

	ImGui::Begin("Gravity Simulation");

	// -----------------------------------------------------------------
	// Visualization toggles
	// -----------------------------------------------------------------
	ImGui::Spacing();
	ImGui::TextUnformatted("Integrator test: steps per orbit (RK4, circular two-body orbit)");
	ImGui::Separator();

	ImGui::InputFloat("mu", &m_TestMu, 0.f, 0.f, "%.4f");
	ImGui::InputFloat("radius", &m_TestRadius, 0.f, 0.f, "%.4f");

	// Only on the button press
	if (ImGui::Button("Run test") && m_TestMu > 0.f && m_TestRadius > 0.f)
	{
		m_Rows = gravity::RunStepConvergence(m_TestMu, m_TestRadius, { 16, 32, 64, 128, 256, 512, 1024 }, { 1, 10, 100 });
	}

	if (!m_Rows.empty())
	{
		ImGui::SameLine();

		if (ImGui::BeginTable("convergence", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
		{
			ImGui::TableSetupColumn("Steps/orbit");
			ImGui::TableSetupColumn("Orbits");
			ImGui::TableSetupColumn("Radius error");
			ImGui::TableSetupColumn("Position error");
			ImGui::TableSetupColumn("Energy drift");
			ImGui::TableHeadersRow();

			for (const auto& row : m_Rows)
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
