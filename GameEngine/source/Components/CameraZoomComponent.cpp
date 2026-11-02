#include "CameraZoomComponent.h"
#include "Renderer.h"
#include "Camera.h"
#include "Services/ServiceLocator.h"
#include "Services/InputManager.h"

#include <imgui.h>
#include <cmath>
#include <glm/glm.hpp>

ge::CameraZoomComponent::CameraZoomComponent(GameObject* ownerPtr, Camera* camera, float zoomStep)
	:Component(ownerPtr),
	m_Camera{ camera },
	m_ZoomStep{ zoomStep }
{
}

void ge::CameraZoomComponent::UpdateComponent(float /*deltaTime*/)
{
    const ImGuiIO& io{ ImGui::GetIO() };

    // Zooming:
    const float wheel{ ge::ServiceLocator::GetInputManager().GetMouseWheelDelta() };
    if (wheel != 0.f && !io.WantCaptureMouse)
    {
        float mx{};
        float my{};

        SDL_GetMouseState(&mx, &my);
        const glm::vec2 anchor{ ge::Renderer::GetInstance().ScreenToWorld({ mx, my }) };

        // Zoom toward the cursor
        m_Camera->ZoomAt(std::pow(m_ZoomStep, wheel), anchor);
    }

    // Panning:
    auto& renderer{ Renderer::GetInstance() };
    const glm::vec2 mouse{ io.MousePos.x, io.MousePos.y };

    if (ImGui::IsMouseDown(ImGuiMouseButton_Middle)) // With middle mouse button
    {
        const glm::vec2 delta{ io.MouseDelta.x, io.MouseDelta.y };
        const glm::vec2 now{ renderer.ScreenToWorld(mouse) };
        const glm::vec2 prev{ renderer.ScreenToWorld(mouse - delta) };
        m_Camera->SetPosition(m_Camera->GetPosition() + (prev - now));
    }
}
