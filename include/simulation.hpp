#pragma once
#include <functional>
#include <fstream>

const double DEFAULT_M1 = 60;
const double DEFAULT_M2 = 10;
const double DEFAULT_V0_KMH = 18;
const double DEFAULT_BETA = 1.2;
const double DEFAULT_H = 15;
const double DEFAULT_L = 60;
const double DEFAULT_DT = 0.001;

const double G = 9.80665;


enum class integration_method { Euler, RK4 };

struct simulation_params_t
{
    double m1 = DEFAULT_M1;
    double m2 = DEFAULT_M2;
    double v0 = DEFAULT_V0_KMH / 3.6;
    double beta = DEFAULT_BETA;
    double H = DEFAULT_H;
    double L = DEFAULT_L;
    double dt = DEFAULT_DT;
    integration_method method = integration_method::Euler;
    std::ofstream* file_stream = nullptr;
};


struct state_t
{
    double x;
    double v;
};

enum class ground_state
{
    finished,
    stopped,
    detached
};





state_t simulate_cyclist(std::function<double(double, simulation_params_t)> func, state_t state, simulation_params_t params);

struct simulation_result_t {
    double final_velocity;
    double time;
    double x;
    ground_state state;
};

simulation_result_t simulate(std::function<double(double,simulation_params_t)> y, simulation_params_t params);

