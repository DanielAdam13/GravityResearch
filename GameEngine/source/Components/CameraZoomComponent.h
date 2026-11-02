#pragma once
#include "Component.h"

namespace ge
{
	class Camera;
	class GameObject;

	// Needs to be attached to the Camera GameObject. 
	class CameraZoomComponent final : public Component
	{
	public:
		// ---- TYPE IDENTIFIER ----
		// Every CameraZoom Instance shares the same component type ID
		static constexpr ComponentTypeID StaticTypeID{ 38 };

		CameraZoomComponent(GameObject* ownerPtr, Camera* camera, float zoomStep = 1.1f);
		~CameraZoomComponent() override = default;
		CameraZoomComponent(const CameraZoomComponent& other) = delete;
		CameraZoomComponent(CameraZoomComponent&& other) = delete;
		CameraZoomComponent& operator=(const CameraZoomComponent& other) = delete;
		CameraZoomComponent& operator=(CameraZoomComponent&& other) = delete;

		virtual void FixedUpdateComponent(float) override {};
		virtual void UpdateComponent(float deltaTime) override;
		virtual void RenderComponent() const override {};

	private:
		Camera* m_Camera; // Non-owning reference
		float m_ZoomStep;
	};
}