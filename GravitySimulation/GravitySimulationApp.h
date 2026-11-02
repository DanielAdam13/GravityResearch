#pragma once
#include "IGameApplication.h"
#include "Camera.h"

#include <memory>

namespace ge
{
    class Scene;
}

namespace customProject
{
    class GravitySimulationApp final : public ge::IGameApplication
    {
    public:
        GravitySimulationApp() = default;
        ~GravitySimulationApp() override = default;
        GravitySimulationApp(const GravitySimulationApp&) = delete;
        GravitySimulationApp(GravitySimulationApp&&) = delete;
        GravitySimulationApp& operator=(const GravitySimulationApp&) = delete;
        GravitySimulationApp& operator=(GravitySimulationApp&&) = delete;

        // ---- IGameApplication ----
        void Load() override;
        void Update(float) override;
        void FixedUpdate(float) override;

    private:
        void BindInputCommands();

        std::unique_ptr<ge::Camera> m_DefaultCamera; // Owned
        void SetupDefaultCamera(ge::Scene& scene);
    };
}
