#pragma once
#include <functional>
#include <limits>

#include "simulation.hpp"

namespace math
{
    const double double_min = std::numeric_limits<double>::lowest();
    const double double_max = -double_min;

    struct min_max_t
    {
        double min, max;
    };

    min_max_t find_min_max(std::function<double(double,simulation_params_t)> func, simulation_params_t params, double x_min, double x_max, double step = 0.001);
}