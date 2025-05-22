#include "cyclist.hpp"


cyclist::cyclist(std::string name, std::string sprite_path, SDL_Color color,  std::function<double(double, simulation_params_t)> func, simulation_params_t params)
{
    this->func = func;
    this->color = color;

    state = { 0, params.v0};
}


void cyclist::simulation_step(double dt)
{
    state = simulate_cyclist(func, state, params);
    x = state.x;
    y = func(x, params);
}
