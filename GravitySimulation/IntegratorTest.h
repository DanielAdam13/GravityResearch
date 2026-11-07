#pragma once
#include <string>
#include <vector>

namespace gravity
{
    struct ConvergenceRow
    {
        int stepsPerOrbit;
        int orbits;
        float radiusError;
        float positionError;
        float energyDrift;
    };

    // Ran once when ImGui button is pressed
    std::vector<ConvergenceRow> RunStepConvergence(float mu, float radius,
        const std::vector<int>& stepCounts, const std::vector<int>& orbitCounts);
}