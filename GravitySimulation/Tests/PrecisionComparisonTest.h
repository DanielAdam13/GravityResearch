#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <numbers>

#include "Scenario.h"

namespace gravity
{
    struct PrecisionRow 
    { 
        double r0;
        double devDouble;
        double devFloat; 
    };

    inline std::vector<PrecisionRow> RunPrecisionComparison(double mu, double phase, double tProp,
        int stepsPerLocal, const std::vector<double>& radii)
    {
        std::vector<PrecisionRow> rows;
        for (const double r0 : radii)
        {
            const Scenario sc{ r0, phase };
            rows.push_back({ r0, MaxRelativeDeviation<double>(mu, sc, tProp, stepsPerLocal),
                                 MaxRelativeDeviation<float>(mu, sc, tProp, stepsPerLocal) });
        }
        return rows;
    }
}