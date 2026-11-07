#include "IntegratorTest.h"
#include "PhysicsCore.h"

#include <cmath>
#include <numbers>
#include <sstream>

std::vector<gravity::ConvergenceRow> gravity::RunStepConvergence(float mu, float radius, const std::vector<int>& stepCounts, const std::vector<int>& orbitCounts)
{
    std::vector<ConvergenceRow> rows;

    const TwoBody model{ mu };
    const float speed{ std::sqrt(mu / radius) }; // Circular orbit speed
    const float period{ 2.f * static_cast<float>( std::numbers::pi ) * std::sqrt(radius * radius * radius / mu) };
    const State initial{ { radius, 0.f }, { 0.f, speed } };
    const float energy0{ 0.5f * glm::dot(initial.velocity, initial.velocity) - mu / radius }; // Initial energy

    for (const int n : stepCounts)
    {
        const float h{ period / n }; // N steps per orbit
        for (const int orbits : orbitCounts)
        {
            State s{ initial };
            for (long long i{ 0 }; i < static_cast<long long>(n) * orbits; ++i)
            {
                s = model.Rk4Step(s, h);
            }

            // New radius
            const float r{ glm::length(s.position) };
            // New energy
            const float e{ 0.5f * glm::dot(s.velocity, s.velocity) - mu / r };

            rows.push_back({ n, orbits,
                std::abs(r - radius),
                glm::length(s.position - initial.position),
                std::abs((e - energy0) / energy0) });
        }
    }
    return rows;
}
