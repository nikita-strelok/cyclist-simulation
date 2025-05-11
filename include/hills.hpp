#pragma once
#include "simulation.hpp"

struct hill_t
{
    const char *desc;
    std::function<double(double, simulation_params_t)> func;
};

namespace hills
{

    double linear(double x, simulation_params_t params);
    double circular_arc_left(double x, simulation_params_t params);
    double circular_arc_right(double x, simulation_params_t params);
    double cosine_modulated(double x, simulation_params_t params);

}