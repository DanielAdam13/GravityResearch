#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <numbers>

#include "PhysicsCore.h"

namespace gravity
{
    struct ConvergenceRow
    {
        int stepsPerOrbit;
        int orbits;
        double radiusError; // Always double because will be displayed in HUD
        double positionError;
        double energyDrift;
    };

    // Ran once when ImGui button is pressed
    template <typename T>
    std::vector<ConvergenceRow> RunStepConvergence(double mu, double radius,
        const std::vector<int>& stepCounts, const std::vector<int>& orbitCounts)
    {
        std::vector<ConvergenceRow> rows;

        const TwoBody<T> model{ static_cast<T>(mu) };
        const double period{ 2.0 * std::numbers::pi *std::sqrt(radius * radius * radius / mu) };
        const T speed{ static_cast<T>(std::sqrt(mu / radius)) }; // Circular orbit speed

        const State<T> initialS{ { static_cast<T>(radius), T(0) }, { T(0), speed } };
        const glm::dvec4 initialSDouble{ ToDouble(initialS) };

        const double radius0{ glm::length(glm::dvec2{ initialSDouble.x, initialSDouble.y }) };
        const double energy0{ helpers::SpecificEnergy(initialSDouble, mu) }; // Physics is in double

        for (const int n : stepCounts)
        {
            const T h{ static_cast<T>(period / n) }; // N steps per orbit
            for (const int orbits : orbitCounts)
            {
                State<T> s{ initialS };
                for (long long i{ 0 }; i < static_cast<long long>(n) * orbits; ++i)
                {
                    s = model.Rk4Step(s, h);
                }

                // Output measurments are in double
                // ----------------------------------------
                const glm::dvec4 doubleS{ ToDouble(s) };
                // New radius
                const double r{ glm::length(glm::dvec2{ doubleS.x, doubleS.y }) };
                // New energy
                const double e{ helpers::SpecificEnergy(doubleS, mu) };
                // ----------------------------------------

                rows.push_back({ n, orbits,
                    std::abs(r - radius0),
                    glm::length(glm::dvec2{ doubleS.x - initialSDouble.x, doubleS.y - initialSDouble.y }),
                    std::abs(e - energy0) / std::abs(energy0) });
            }
        }
        return rows;
    }
}