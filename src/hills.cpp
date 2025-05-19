#include "hills.hpp"
#include <cmath>


double hills::linear(double x, simulation_params_t params)
{
    return (params.H / params.L) * (params.L - x);
}

double hills::circular_arc_right(double x, simulation_params_t params)
{
    double H = params.H;
    double L = params.L;
    
    double R = (H * H + L * L) / (2 * H);
    return R - sqrt(R * R - (L - x) * (L - x));
}

double hills::circular_arc_left(double x, simulation_params_t params)
{
    double H = params.H;
    double L = params.L;

    double R = (H * H + L * L) / (2 * H);
    return H - R + sqrt(R * R - x * x);
}

double hills::cosine_modulated(double x, simulation_params_t params)
{
    return (params.H / params.L) * (params.L - x) * cos(2 * PI * x / params.L);
}