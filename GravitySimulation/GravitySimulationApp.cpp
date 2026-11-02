#include "GravitySimulationApp.h"

#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Components/Transform.h"
#include "Components/TextComponent.h"
#include "Components/CameraZoomComponent.h"
#include "Components/Image.h"

#include "Font.h"
#include "Texture2D.h"
#include "ResourceManager.h"

#include "Renderer.h"
#include "Services/ServiceLocator.h"
#include "Services/InputManager.h"
#include "Commands/ChangeWindowSizeCommand.h"

#include <memory>
#include <utility>

void customProject::GravitySimulationApp::Load()
{
	ge::Renderer::GetInstance().SetWindowSize({ 800, 800 });

	// 1. Scene setup - a project needs at least one scene to hold its GameObjects
	ge::Scene& scene{ ge::SceneManager::GetInstance().CreateScene("MainScene") };
	ge::SceneManager::GetInstance().SwitchToSceneWithName("MainScene");

	// 2. GO Creation
	ge::Texture2D* satelliteTexture{ ge::ResourceManager::GetInstance().LoadTexture("satellite.png") };
	auto satelliteObject{ std::make_unique<ge::GameObject>("SatelliteGO") };
	satelliteObject->AddComponent<ge::Image>(satelliteObject.get())->SetTexture(satelliteTexture);
	satelliteObject->GetComponent<ge::Transform>()->SetLocalScale(0.1f, 0.1f, 0.1f);

	scene.Add(std::move(satelliteObject));

	// 3. Default Camera Setup
	SetupDefaultCamera(scene);

	// 4. Bind commands
	BindInputCommands();
}

void customProject::GravitySimulationApp::Update(float)
{
}

void customProject::GravitySimulationApp::FixedUpdate(float)
{
}

void customProject::GravitySimulationApp::BindInputCommands()
{
	auto& input{ ge::ServiceLocator::GetInputManager() };

	// F1: toggle between current window size and 1200x1200
	input.BindKeyboardCommand(SDL_SCANCODE_F1,
		ge::InputManager::InputTrigger::Up,
		std::make_unique<ge::ChangeWindowSizeCommand>(1200, 1200));
}

void customProject::GravitySimulationApp::SetupDefaultCamera(ge::Scene& scene)
{
	m_DefaultCamera = std::make_unique<ge::Camera>(glm::vec2{
		static_cast<float>(ge::Renderer::GetInstance().GetWindowDesignSize().first),
		static_cast<float>(ge::Renderer::GetInstance().GetWindowDesignSize().second) });
	m_DefaultCamera->SetPosition({ 0.f, 0.f });

	ge::Renderer::GetInstance().SetActiveCamera(m_DefaultCamera.get());

	auto cameraGO{ std::make_unique<ge::GameObject>("GO_Camera") };
	cameraGO->AddComponent<ge::CameraZoomComponent>(cameraGO.get(), m_DefaultCamera.get(), 1.1f);

	scene.Add(std::move(cameraGO));
}
