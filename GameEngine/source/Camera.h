#pragma once

#include <glm/glm.hpp>
#include <limits>

namespace ge
{
	class Camera final
	{
	public:
		Camera(const glm::vec2& viewportSize);
		~Camera(); // Clears itself from the Renderer if this == activeCamera
		Camera(const Camera&) = delete;
		Camera(Camera&&) = delete;
		Camera& operator=(const Camera&) = delete;
		Camera& operator=(Camera&&) = delete;

		void SetPosition(const glm::vec2& worldPos);
		const glm::vec2& GetPosition() const noexcept { return m_Position; }

		void SetBounds(const glm::vec2& min, const glm::vec2& max);

		// Anchor world is fixed on screen (under mouse)
		void ZoomAt(float factor, const glm::vec2& anchorWorld);

		void SetZoom(float zoom);
		float GetZoom() const noexcept { return m_Zoom; }

		// Helpers converting transforms from ZOOM.
		// Used in Renderer
		// --------------------
		glm::vec2 WorldToView(const glm::vec2& world) const noexcept;
		glm::vec2 ViewToWorld(const glm::vec2& view) const noexcept;
		// --------------------

	private:
		glm::vec2 m_Position{ 0.f, 0.f };
		glm::vec2 m_ViewportSize;
		glm::vec2 m_ViewportCenter;

		glm::vec2 m_BoundsMin{ -floatInfinity, -floatInfinity };
		glm::vec2 m_BoundsMax{ floatInfinity, floatInfinity };
		bool m_HasBounds{ false };

		float m_Zoom{ 1.f };
		static constexpr float MinZoom{ 0.01f };
		static constexpr float MaxZoom{ 5000.f };

		static constexpr float floatInfinity{ std::numeric_limits<float>().infinity() };

		void ApplyBounds();
	};
}