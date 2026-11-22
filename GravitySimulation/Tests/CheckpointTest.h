#pragma once
#include "Scenario.h"

#include <string>
#include <vector>
#include <sstream>
#include <numbers>

namespace gravity
{
    struct CheckResult
    {
        std::string name;
        double value;
        double limit;
        bool pass;
    };

    inline std::vector<CheckResult> RunCheckpoints(double mu)
    {
        constexpr double twoPi{ 2.0 * std::numbers::pi };
        std::vector<CheckResult> out;

        // Does the PCR3BP model match the initial state of Two-Body at t = 0 (it should) and if frame conversion consistent?
        double worst{ 0.0 };
        for (const double phase : { 0.0, 0.7, std::numbers::pi / 2, std::numbers::pi, 4.71 })
        {
            const Scenario sc{ 0.1, phase };
            const auto a{ RotatingToEathCentered(Pcr3bpStart(sc, mu), mu, 0.0) };
            const auto b{ TwoBodyStart(sc, mu) };
            worst = std::max({ worst, glm::length(a.position - b.position), glm::length(a.velocity - b.velocity) });
        }
        out.push_back({ "Conversion at t=0", worst, 1e-13, worst < 1e-13 });

        // Do both models agree if the physics are right for when mu = 0 (Moon’s mass is 0)?
        const double d200{ MaxRelativeDeviation<double>(1e-12, { 0.1, 0.0 }, twoPi, 200) };
        const double d400{ MaxRelativeDeviation<double>(1e-12, { 0.1, 0.0 }, twoPi, 400) };
        out.push_back({ "mu->0 deviation, N=200", d200, 1e-4, d200 < 1e-4 });
        out.push_back({ "mu->0 halving ratio",   d200 / d400, 16.0, d200 / d400 > 16.0 });

        return out;
    }
}