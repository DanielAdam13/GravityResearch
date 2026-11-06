#include "IntegratorTest.h"
#include "PhysicsCore.h"

#include <cmath>
#include <numbers>
#include <sstream>

std::vector<gravity::ConvergenceRow> gravity::RunStepConvergence(double mu, double radius, const std::vector<int>& stepCounts, const std::vector<int>& orbitCounts)
{
    std::vector<ConvergenceRow> rows;

    const TwoBody model{ mu };
    const double speed{ std::sqrt(mu / radius) }; // Circular orbit speed
    const double period{ 2.0 * std::numbers::pi * std::sqrt(radius * radius * radius / mu) };
    const State initial{ { radius, 0.0 }, { 0.0, speed } };
    const double energy0{ 0.5 * glm::dot(initial.velocity, initial.velocity) - mu / radius }; // Initial energy

    for (const int n : stepCounts)
    {
        const double h{ period / n }; // N steps per orbit
        for (const int orbits : orbitCounts)
        {
            State s{ initial };
            for (long long i{ 0 }; i < static_cast<long long>(n) * orbits; ++i)
            {
                s = model.Rk4Step(s, h);
            }

            // New radius
            const double r{ glm::length(s.position) };
            // New energy
            const double e{ 0.5 * glm::dot(s.velocity, s.velocity) - mu / r };

            rows.push_back({ n, orbits,
                std::abs(r - radius),
                glm::length(s.position - initial.position),
                std::abs((e - energy0) / energy0) });
        }
    }
    return rows;
}
