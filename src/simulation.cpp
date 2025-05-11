#include "simulation.hpp"
#include <fstream>
#include <cmath>

double derivative(std::function<double(double)> f, double x, double h = 1e-6)
{
    return (f(x + h) - f(x - h)) / (2 * h);
}
double derivative2(std::function<double(double)> f, double x, double h = 1e-6)
{
    return (f(x + h) - 2 * f(x) + f(x - h)) / (h * h);
}

simulation_result_t simulate(std::function<double(double, simulation_params_t)> func, simulation_params_t params)
{

    double dt = params.dt;
    double t = 0, x = 0, v = params.v0;
    double m1 = params.m1;
    double M = params.m1 + params.m2;
    double L = params.L;
    double beta = params.beta;

    auto y = [func, params](double x)
    { return func(x, params); };

    std::ofstream file("trajectory.txt");

    while (x <= L)
    {

        double dy = derivative(y, x);
        double ddy = derivative2(y, x);
        double theta = atan(dy);

        double R = pow(1 + dy * dy, 3.0 / 2.0) / fabs(ddy);

        double N = M * G * cos(theta) - M * v * v / R;

        double Fg = M * G * sin(fabs(theta)) * (dy < 0 ? 1 : -1);
        double Fd = beta * pow(m1, (2.0 / 3.0)) * v;
        double a = (Fg - Fd) / M;

        file << t << " " << x << "\n";

        if (N <= 0)
        {
            return {v, t, x, ground_state::detached};
        }

        v += a * dt;
        x += v * dt;
        t += dt;

        if (v <= 1e-6)
        {
            return {v, t, x, ground_state::stopped};
        }
    }
    file.close();

    return {v, t, x, ground_state::finished};
}