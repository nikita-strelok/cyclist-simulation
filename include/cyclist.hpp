#pragma once
#include <functional>
#include <string>
#include <SDL3/SDL.h>

#include "simulation.hpp"

class cyclist
{

public:
    cyclist(std::string name, std::string sprite_path, SDL_Color color,  std::function<double(double, simulation_params_t)> func, simulation_params_t params);
    
    double get_x() const { return x; }
    double get_y() const { return y; }
    void simulation_step(double dt);

    SDL_Color get_color() const { return color; }
    std::function<double(double,simulation_params_t)> func;
    SDL_Color color;
    state_t state { 0, 0};
    simulation_params_t params;

private:
    double x = 0.0;
    double y = 0.0;
    SDL_Texture* texture;
    
};
