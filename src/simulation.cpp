#include "simulation.hpp"
#include <cmath>

double derivative(std::function<double(double)> f, double x, double h = 1e-6)
{
    return (f(x + h) - f(x - h)) / (2 * h);
}
double derivative2(std::function<double(double)> f, double x, double h = 1e-6)
{
    return (f(x + h) - 2 * f(x) + f(x - h)) / (h * h);
}

state_t derivatives(const state_t &s, std::function<double(double)> y, simulation_params_t params)
{
    double M = params.m1 + params.m2;
    double dy = derivative(y, s.x);

    // theta = arctg(dy)
    // sin(theta) = dy/sqrt(1+dy*dy)

    double Fg = M * G * -(dy / sqrt(1 + dy * dy));
    double Fd = params.beta * pow(params.m1, 2.0 / 3.0) * s.v;

    double a = (Fg - Fd) / M;

    return {s.v, a};
}

// Метод Эйлера
state_t euler_step(const state_t &s, std::function<double(double)> y, simulation_params_t params)
{
    state_t deriv = derivatives(s, y, params);
    return {
        s.x + deriv.x * params.dt,
        s.v + deriv.v * params.dt};
}

// Метод Рунге–Кутты 4-го порядка
state_t rk4_step(const state_t &s, std::function<double(double)> y, simulation_params_t params)
{
    auto derivs = [&](const state_t &state)
    {
        return derivatives(state, y, params);
    };

    state_t k1 = derivs(s);
    state_t k2 = derivs({s.x + k1.x * params.dt / 2, s.v + k1.v * params.dt / 2});
    state_t k3 = derivs({s.x + k2.x * params.dt / 2, s.v + k2.v * params.dt / 2});
    state_t k4 = derivs({s.x + k3.x * params.dt, s.v + k3.v * params.dt});

    return {
        s.x + (k1.x + 2 * k2.x + 2 * k3.x + k4.x) * params.dt / 6,
        s.v + (k1.v + 2 * k2.v + 2 * k3.v + k4.v) * params.dt / 6};
}

state_t simulate_cyclist(std::function<double(double, simulation_params_t)> func, state_t state, simulation_params_t params)
{
    state_t (*integration_step)(const state_t &, std::function<double(double)>, simulation_params_t);

    if (params.method == integration_method::Euler)
    {
        integration_step = euler_step;
    }
    else
    {
        integration_step = rk4_step;
    }

    auto y = [func, params](double x)
    { return func(x, params); };

    double dy = derivative(y, state.x);
    double ddy = derivative2(y, state.x);
    double theta = atan(dy);

    double R = pow(1 + dy * dy, 3.0 / 2.0) / fabs(ddy);
    double N = (params.m1 + params.m2) * G * cos(theta) - (params.m1 + params.m2) * state.v * state.v / R;

    if (N <= 1e-6)
    {
        
    }

    if (state.v <= 1e-6)
    {
        
    }

    return integration_step(state, y, params);
}

simulation_result_t simulate(std::function<double(double, simulation_params_t)> func, simulation_params_t params)
{
    double t = 0.0;
    state_t state = {0.0, params.v0};

    auto y = [func, params](double x)
    { return func(x, params); };

    state_t (*integration_step)(const state_t &, std::function<double(double)>, simulation_params_t);

    if (params.method == integration_method::Euler)
    {
        integration_step = euler_step;
    }
    else
    {
        integration_step = rk4_step;
    }

    while (state.x <= params.L + 1e-6)
    {
        if (params.file_stream)
        {
            *params.file_stream << t << " " << state.x << " " << y(state.x) << "\n";
        }

        double dy = derivative(y, state.x);
        double ddy = derivative2(y, state.x);
        double theta = atan(dy);

        double R = pow(1 + dy * dy, 3.0 / 2.0) / fabs(ddy);
        double N = (params.m1 + params.m2) * G * cos(theta) - (params.m1 + params.m2) * state.v * state.v / R;

        if (N <= 1e-6)
        {
            return {state.v, t, state.x, ground_state::detached};
        }

        if (state.v <= 1e-6)
        {
            return {state.v, t, state.x, ground_state::stopped};
        }

        state = integration_step(state, y, params);
        t += params.dt;
    }

    return {state.v, t, state.x, ground_state::finished};
}

