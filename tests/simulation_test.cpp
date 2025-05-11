#include <gtest/gtest.h>
#include <cmath>
#include <iostream>

#include "simulation.hpp"
#include "hills.hpp"

TEST(SimulationTest, NoFrictionDownhill_SpeedMatchesTheory) {
    double m1 = 60;
    double m2 = 10;
    double v0 = 18.0 / 3.6;
    double beta = 0.0;
    double H = 15.0;
    double L = 60.0;
    double dt = 0.001;

    simulation_params_t params = {
        .m1 = m1,
        .m2 = m2,
        .v0 = v0,
        .beta = beta,
        .H = H,
        .L = L,
        .dt = dt
    };

   
    auto result = simulate(hills::linear, params);

    double expected_vf = sqrt(v0 * v0 + 2 * G * H);
    double actual_vf = result.final_velocity;

    
    double percent_diff = fabs((actual_vf - expected_vf) / expected_vf) * 100.0;

    
    std::cout << "Теоретическая скорость: " << expected_vf << " м/с\n";
    std::cout << "Фактическая скорость:  " << actual_vf << " м/с\n";
    std::cout << "Процентное отклонение: " << percent_diff << " %\n";

    
    EXPECT_LT(percent_diff, 5.0);
       
}