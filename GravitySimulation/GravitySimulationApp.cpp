#include "GravitySimulationApp.h"

#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Components/Transform.h"
#include "Components/TextComponent.h"
#include "Components/CameraZoomComponent.h"
#include "Components/Image.h"

#include "Components/SimulationComponent.h"
#include "Components/CelestialBodyComponent.h"

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

	// 2. Game Object Creation

	// Satellite
	ge::Texture2D* satelliteTexture{ ge::ResourceManager::GetInstance().LoadTexture("satellite.png") };
	auto satelliteObject{ std::make_unique<ge::GameObject>("SatelliteGO") };
	auto* imageCompS{ satelliteObject->AddComponent<ge::Image>(satelliteObject.get()) };
	imageCompS->SetTexture(satelliteTexture);
	imageCompS->SetAnchor({ 0.5f, 0.5f });
	satelliteObject->GetComponent<ge::Transform>()->SetLocalScale(0.1f, 0.1f, 0.1f);

	const State initialSatState{ { 3.0, 0.0 }, { 0.0, 1.0 } };
	const BodyData satelliteData{ "Satellite", 0.0, 0.0 };
	auto* bodyCompS{ satelliteObject->AddComponent<CelestialBodyComponent>(satelliteObject.get(), satelliteData, initialSatState) };

	scene.Add(std::move(satelliteObject));
	
	// Earth - central body
	ge::Texture2D* earthTexture{ ge::ResourceManager::GetInstance().LoadTexture("earth.png") };
	auto earthObject{ std::make_unique<ge::GameObject>("EarthGO") };
	auto* imageCompE{ earthObject->AddComponent<ge::Image>(earthObject.get()) };
	imageCompE->SetTexture(earthTexture);
	imageCompE->SetAnchor({ 0.5f, 0.5f });
	earthObject->GetComponent<ge::Transform>()->SetLocalScale(1.f, 1.f, 1.f);

	const State initialEarthState{ { 6.0, 0.0 }, { 0.0, 0.0 } };
	const BodyData earthData{ "Earth", 2.0, 0.0 };
	auto* bodyCompE{ earthObject->AddComponent<CelestialBodyComponent>(earthObject.get(), earthData, initialEarthState) };

	scene.Add(std::move(earthObject));

	// Simulation GO
	auto simultionObject{ std::make_unique<ge::GameObject>("SimulationGO") };
	const double T{ helpers::OrbitPeriod(
		helpers::SemiMajorAxis({initialSatState.position, initialSatState.velocity}, earthData.mu), earthData.mu) };
	const double h{ T / SimulationSteps };
	simultionObject->AddComponent<SimulationComponent>(simultionObject.get(),
		bodyCompE, bodyCompS,
		h);

	scene.Add(std::move(simultionObject));
	

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

	m_DefaultCamera->SetZoom(0.2f);

	ge::Renderer::GetInstance().SetActiveCamera(m_DefaultCamera.get());

	auto cameraGO{ std::make_unique<ge::GameObject>("GO_Camera") };
	cameraGO->AddComponent<ge::CameraZoomComponent>(cameraGO.get(), m_DefaultCamera.get(), 1.1f);

	scene.Add(std::move(cameraGO));
}
