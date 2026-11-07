#include "GravitySimulationApp.h"

#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Components/Transform.h"
#include "Components/TextComponent.h"
#include "Components/CameraZoomComponent.h"
#include "Components/Image.h"
#include "Components/SimulationComponent.h"

#include "Font.h"
#include "Texture2D.h"
#include "ResourceManager.h"

#include "Renderer.h"
#include "Services/ServiceLocator.h"
#include "Services/InputManager.h"
#include "Commands/ChangeWindowSizeCommand.h"

#include "PhysicsCore.h"
#include "SimulationHUDImGui.h"
#include "PhysicsHelpers.h"

#include <memory>
#include <utility>

void gravity::GravitySimulationApp::Load()
{
	ge::Renderer::GetInstance().SetWindowSize({ 800, 800 });

	// 1. Scene setup - a project needs at least one scene to hold its GameObjects
	ge::Scene& scene{ ge::SceneManager::GetInstance().CreateScene("MainScene") };
	ge::SceneManager::GetInstance().SwitchToSceneWithName("MainScene");

	// 2. GO Creation
	ge::Texture2D* satelliteTexture{ ge::ResourceManager::GetInstance().LoadTexture("satellite.png") };
	auto satelliteObject{ std::make_unique<ge::GameObject>("SatelliteGO") };
	auto* imageCompS{ satelliteObject->AddComponent<ge::Image>(satelliteObject.get()) };
	imageCompS->SetTexture(satelliteTexture);
	imageCompS->SetAnchor({ 0.5f, 0.5f });
	satelliteObject->GetComponent<ge::Transform>()->SetLocalScale(0.1f, 0.1f, 0.1f);

	constexpr float mu{ 5.f };
	const State initialState{ { 5.f, 0.f }, { 0.f, 1.f } };
	const float T{ helpers::OrbitPeriod(
		helpers::SemiMajorAxis({initialState.position, initialState.velocity}, mu), mu) };
	const float h{ T / SimulationSteps };
	satelliteObject->AddComponent<SimulationComponent>(satelliteObject.get(), 
		std::make_unique<TwoBody>(mu),
		initialState,
		h);

	scene.Add(std::move(satelliteObject));

	// Earth
	ge::Texture2D* earthTexture{ ge::ResourceManager::GetInstance().LoadTexture("earth.png") };
	auto earthObject{ std::make_unique<ge::GameObject>("SatelliteGO") };
	auto* imageCompE{ earthObject->AddComponent<ge::Image>(earthObject.get()) };
	imageCompE->SetTexture(earthTexture);
	imageCompE->SetAnchor({ 0.5f, 0.5f });
	earthObject->GetComponent<ge::Transform>()->SetLocalScale(1.f, 1.f, 1.f);

	scene.Add(std::move(earthObject));

	// 3. Default Camera Setup
	SetupDefaultCamera(scene);

	// 4. ImGui control panel.
	scene.AddImGuiScene(std::make_unique<SimulationHUDImGui>(this));

	// 5. Bind commands
	BindInputCommands();
}

void gravity::GravitySimulationApp::Update(float)
{
}

void gravity::GravitySimulationApp::FixedUpdate(float)
{
}

void gravity::GravitySimulationApp::BindInputCommands()
{
	auto& input{ ge::ServiceLocator::GetInputManager() };

	// F1: toggle between current window size and 1200x1200
	input.BindKeyboardCommand(SDL_SCANCODE_F1,
		ge::InputManager::InputTrigger::Up,
		std::make_unique<ge::ChangeWindowSizeCommand>(1200, 1200));
}

void gravity::GravitySimulationApp::SetupDefaultCamera(ge::Scene& scene)
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
