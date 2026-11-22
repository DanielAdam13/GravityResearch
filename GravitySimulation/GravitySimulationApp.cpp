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
#include "Components/TrailComponent.h"

#include "Font.h"
#include "Texture2D.h"
#include "ResourceManager.h"

#include "Renderer.h"
#include "Services/ServiceLocator.h"
#include "Services/InputManager.h"
#include "Commands/ChangeWindowSizeCommand.h"

#include "PhysicsModels.h"
#include "SimulationHUDImGui.h"
#include "PhysicsHelpers.h"
#include "SimulationConstants.h"
#include "Scenario.h"

#include <memory>
#include <utility>
#include <numbers>
#include <string>

void gravity::GravitySimulationApp::Load()
{
	ge::Renderer::GetInstance().SetWindowSize({ 1200, 1200 });

	// 1. Scene setup - a project needs at least one scene to hold its GameObjects
	ge::Scene& scene{ ge::SceneManager::GetInstance().CreateScene("MainScene") };
	ge::SceneManager::GetInstance().SwitchToSceneWithName("MainScene");

	// 2. Game Object Creation

	const BodyData earthData{ "Earth", physConstants::EarthMu, physConstants::EarthRadiusKm / physConstants::EarthMoonDistanceKm };
	const BodyData moonData{ "Moon", physConstants::MoonMu, physConstants::MoonRadiusKm / physConstants::EarthMoonDistanceKm };
	const double massRatio{ moonData.mu / (earthData.mu + moonData.mu) };
	
	// Earth - central body
	ge::Texture2D* earthTexture{ ge::ResourceManager::GetInstance().LoadTexture("earth.png") };
	auto earthObject{ std::make_unique<ge::GameObject>("EarthGO") };
	auto* imageCompE{ earthObject->AddComponent<ge::Image>(earthObject.get()) };
	imageCompE->SetTexture(earthTexture);
	imageCompE->SetAnchor({ 0.5f, 0.5f });
	earthObject->GetComponent<ge::Transform>()->SetLocalScale(1.f, 1.f, 1.f);
	
	const State<double> initialEarthState{ { -massRatio, 0.0 }, { 0.0, 0.0 } };
	earthObject->AddComponent<CelestialBodyComponent>(earthObject.get(), earthData, initialEarthState)->
		FitSpriteToRadius(simConstants::BodyDisplayExaggeration, simConstants::MinBodyDiameterWorld);

	scene.Add(std::move(earthObject));

	// Moon - body
	ge::Texture2D* moonTexture{ ge::ResourceManager::GetInstance().LoadTexture("Moon.png") };
	auto moonObject{ std::make_unique<ge::GameObject>("MoonGO") };
	auto* imageCompM{ moonObject->AddComponent<ge::Image>(moonObject.get()) };
	imageCompM->SetTexture(moonTexture);
	imageCompM->SetAnchor({ 0.5f, 0.5f });

	const State<double> initialMoonState{ { (1 - massRatio), 0.0}, {0.0, 0.0} };
	moonObject->AddComponent<CelestialBodyComponent>(moonObject.get(), moonData, initialMoonState)->
		FitSpriteToRadius(simConstants::BodyDisplayExaggeration, simConstants::MinBodyDiameterWorld);

	scene.Add(std::move(moonObject));

	// Satellite
	ge::Texture2D* satelliteTexture{ ge::ResourceManager::GetInstance().LoadTexture("satellite.png") };
	auto satelliteObject{ std::make_unique<ge::GameObject>("SatelliteGO") };
	auto* imageCompS{ satelliteObject->AddComponent<ge::Image>(satelliteObject.get()) };
	imageCompS->SetTexture(satelliteTexture);
	imageCompS->SetAnchor({ 0.5f, 0.5f });
	satelliteObject->GetComponent<ge::Transform>()->SetLocalScale(0.1f, 0.1f, 0.1f);
	satelliteObject->AddComponent<TrailComponent>(satelliteObject.get(), 5000, 0.02, SDL_Color{ 255, 200, 0, 255 });

	const State<double> initialSatState{ { 1.15568, 0 }, { 0.0, 0.0 } };
	const BodyData satelliteData{ "Satellite", 0.0, 0.0 }; // 0 grav paremeter since it is negligible, radius = 0 -> a point
	auto* bodyCompS{ satelliteObject->AddComponent<CelestialBodyComponent>(satelliteObject.get(), satelliteData, initialSatState) };
	bodyCompS->FitSpriteToRadius(simConstants::BodyDisplayExaggeration, simConstants::MinBodyDiameterWorld);

	scene.Add(std::move(satelliteObject));

	// Simulation GO
	auto simultionObject{ std::make_unique<ge::GameObject>("SimulationGO") };

	/*const double T{ helpers::OrbitPeriod(
		helpers::SemiMajorAxis({initialSatState.position, initialSatState.velocity}, earthData.mu), earthData.mu) };
	const double htWO{ T / simConstants::SimulationSteps };
	auto* simComp{ simultionObject->AddComponent<SimulationComponent>(simultionObject.get(),
		std::make_unique<TwoBody<double>>(massRatio), bodyCompS,
		htWO) };*/

	constexpr double hPcr3bp{ 2 * std::numbers::pi_v<double> / simConstants::StepsPerPrimaryPeriod };
	auto* simComp{ simultionObject->AddComponent<SimulationComponent>(simultionObject.get(),
		std::make_unique<Pcr3bp<double>>(massRatio), bodyCompS,
		hPcr3bp) };

	scene.Add(std::move(simultionObject));

	ge::Font* font{ ge::ResourceManager::GetInstance().LoadFont("YourFont.otf", 24) };
	// Conserved Quantity Text GO
	auto consQuantTextObject{ std::make_unique<ge::GameObject>("ConservedQuantityGO") };
	consQuantTextObject->SetIgnoreCamera(true);
	consQuantTextObject->AddComponent<ge::TextComponent>(consQuantTextObject.get(), 
		std::to_string(simComp->GetModelConservedQuantity()), font);
	scene.Add(std::move(consQuantTextObject));
	

	// 3. Default Camera Setup
	SetupDefaultCamera(scene);

	// 4. ImGui control panel.
	scene.AddImGuiScene(std::make_unique<SimulationHUDImGui>(this));
	scene.AddImGuiScene(std::make_unique<SimulationHUDImGui2>(this));
	scene.AddImGuiScene(std::make_unique<SimulationHUDImGui3>(this));

	auto result{ MaxRelativeDeviation<float>(0.0121506, Scenario{0.1, 0.0}, 2 * std::numbers::pi, 1000) };
	result;

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
