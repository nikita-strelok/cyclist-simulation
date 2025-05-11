#include <gtest/gtest.h>
#include <cmath>
#include <iostream>

#include "simulation.hpp"
#include "hills.hpp"

simulation_params_t test_parms
{
    .m1 = 60,
    .m2 = 10,
    .v0 = 180.0 / 3.6,
    .beta = 0,
    .H = 15,
    .L = 60,
    .dt = 0.001,
    .method = integration_method::Euler};

TEST(SimulationTest, NoFrictionDownhill_SpeedMatchesTheoryEuler)
{
    
    simulation_params_t params = test_parms;
    params.method = integration_method::Euler;

    auto result = simulate(hills::linear, params);

    double expected_vf = sqrt(params.v0 * params.v0 + 2 * G * params.H);
    double actual_vf = result.final_velocity;

    double percent_diff = fabs((actual_vf - expected_vf) / expected_vf) * 100.0;

    std::cout << "Теоретическая скорость: " << expected_vf << " м/с\n";
    std::cout << "Фактическая скорость:  " << actual_vf << " м/с\n";
    std::cout << "Процентное отклонение: " << percent_diff << " %\n";

    EXPECT_LT(percent_diff, 5.0);
}

TEST(SimulationTest, NoFrictionDownhill_SpeedMatchesTheoryRK4)
{
    
    simulation_params_t params = test_parms;
    params.method = integration_method::RK4;

    auto result = simulate(hills::linear, params);

    double expected_vf = sqrt(params.v0 * params.v0 + 2 * G * params.H);
    double actual_vf = result.final_velocity;

    double percent_diff = fabs((actual_vf - expected_vf) / expected_vf) * 100.0;

    std::cout << "Теоретическая скорость: " << expected_vf << " м/с\n";
    std::cout << "Фактическая скорость:  " << actual_vf << " м/с\n";
    std::cout << "Процентное отклонение: " << percent_diff << " %\n";

    EXPECT_LT(percent_diff, 5.0);
}