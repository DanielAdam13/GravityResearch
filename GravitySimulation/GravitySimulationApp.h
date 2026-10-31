#pragma once
#include "IGameApplication.h"


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
    };
}
