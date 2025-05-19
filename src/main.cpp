#include <iostream>
#include <functional>
#include <string>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <locale>
#endif

#include "simulation.hpp"
#include "hills.hpp"

hill_t hills_list[]{
    {"y(x) = (H/L)*(L-x)", hills::linear},
    {"y(x) = R-sqrt(R*R-(L-x)*(L-x)); R=(H*H+L*L)/(2*H)", hills::circular_arc_left},
    {"y(x) = H-R+sqrt(R*R-x*x); R=(H*H+L*L)/(2*H)", hills::circular_arc_right},
    {"y(x) = (H/L)*(L-x)*cos(2*pi*x/L)", hills::cosine_modulated}

};

const int hill_count = sizeof(hills_list) / sizeof(hills_list[0]);

#define HELP_ARG "--help"
#define HILLS_ARG "--list-hills"
#define HILL_ARG "--hill="
#define TRAJECTORY_ARG "--plot-trajectory"
#define M1_ARG "--m1="
#define M2_ARG "--m2="
#define V0_ARG "--v0="
#define BETA_ARG "--beta="
#define H_ARG "--H="
#define L_ARG "--L="
#define DT_ARG "--dt="

void print_help()
{
    std::cout << "Доступные аргументы:\n";
    std::cout << HELP_ARG << "\tОтобразить эту информацию\n";
    std::cout << HILLS_ARG << "\tОтобразить список встроеных холмов\n";
    std::cout << HILL_ARG << "<число | строка>\tУказать уравнение холма для симуляции (0-" << hill_count - 1 << ") (по умолчанию 0)\n";
    std::cout << TRAJECTORY_ARG << "\t По окончанию рассчетов построить зависимоть x(t) в gnuplot\n";
    std::cout << M1_ARG << "<число>\t Задать массу велосипедиста в кг (по умочанию: " << DEFAULT_M1 << ")\n";
    std::cout << M2_ARG << "<число>\t Задать массу велосипеда в кг (по умочанию: " << DEFAULT_M2 << ")\n";
    std::cout << V0_ARG << "<число>\t Задать начальную скорость велосипедиста в км/ч (по умочанию: " << DEFAULT_V0_KMH << ")\n";
    std::cout << BETA_ARG << "<число>\tЗадать коэффициент сопротивления (по умолчанию: " << DEFAULT_BETA << ")\n";
    std::cout << H_ARG << "<число>\t Задать высоту холма в м (по умочанию: " << DEFAULT_H << ")\n";
    std::cout << L_ARG << "<число>\t Задать длину холма в м (по умочанию: " << DEFAULT_L << ")\n";
    std::cout << DT_ARG << "<число>\tЗадать шаг симуляции в с (по умолчанию: " << DEFAULT_DT << ")\n";
}

void print_hills()
{
    for (unsigned int i = 0; i < hill_count; i++)
    {
        std::cout << i << '.' << hills_list[i].desc << '\n';
    }
}

int main(int argc, char *argv[])
{

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::locale::global(std::locale(""));
#endif

    bool draw_trajectory = false;
    int hill_index = 0;
    simulation_params_t params{};

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == HELP_ARG)
        {
            print_help();
            return 0;
        }
        else if (arg == HILLS_ARG)
        {
            print_hills();
            return 0;
        }
        else if (arg.find(HILL_ARG) == 0)
        {
            hill_index = std::stoi(arg.substr(strlen(HILL_ARG)));
        }
        else if (arg == TRAJECTORY_ARG)
        {
            draw_trajectory = true;
        }
        else if (arg.find(M1_ARG) == 0)
        {
            params.m1 = std::stod(arg.substr(strlen(M1_ARG)));
        }
        else if (arg.find(M2_ARG) == 0)
        {
            params.m2 = std::stod(arg.substr(strlen(M2_ARG)));
        }
        else if (arg.find(V0_ARG) == 0)
        {
            params.v0 = std::stod(arg.substr(strlen(V0_ARG))) / 3.6;
        }
        else if (arg.find(BETA_ARG) == 0)
        {
            params.beta = std::stod(arg.substr(strlen(BETA_ARG)));
        }
        else if (arg.find(H_ARG) == 0)
        {
            params.H = std::stod(arg.substr(strlen(H_ARG)));
        }
        else if (arg.find(L_ARG) == 0)
        {
            params.L = std::stod(arg.substr(strlen(L_ARG)));
        }
        else if (arg.find(DT_ARG) == 0)
        {
            params.L = std::stod(arg.substr(strlen(DT_ARG)));
        }
        else
        {
            std::cerr << "Неизвестный аргумент: " << arg << "\n";
            return 1;
        }
    }

    auto hill_profile = hills_list[hill_index % hill_count];

    auto result = simulate(hill_profile.func, params);

    if (result.state == ground_state::detached)
    {
        std::cout << "Отрыв от земли\n";
    }
    else if (result.state == ground_state::stopped)
    {
        std::cout << "Полная остановка\n";
    }
    else
    {
        std::cout << "Финиш\n";
    }

    std::cout << "t = " << result.time << " c\n";
    std::cout << "v = " << result.final_velocity << "м/c (" << result.final_velocity * 3.6 << " км/ч)\n";
    std::cout << "x = " << result.x << " м\n";

    if (draw_trajectory)
    {
        std::string path = "gnuplot -p -e \"set key off; set xlabel 't'; set ylabel 'x'; plot 'trajectory.txt' with lines\"";
        popen(path.c_str(), "w");
    }

    return 0;
}
