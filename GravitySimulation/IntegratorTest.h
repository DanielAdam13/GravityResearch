#pragma once
#include <string>
#include <vector>

namespace gravity
{
    struct ConvergenceRow
    {
        int stepsPerOrbit;
        int orbits;
        double radiusError;
        double positionError;
        double energyDrift;
    };

    // Ran once when ImGui button is pressed
    std::vector<ConvergenceRow> RunStepConvergence(double mu, double radius,
        const std::vector<int>& stepCounts, const std::vector<int>& orbitCounts);
}