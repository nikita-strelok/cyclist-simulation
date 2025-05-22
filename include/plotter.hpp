#pragma once
#include <SDL3/SDL.h>
#include <vector>

#include "simulation.hpp"
#include "cyclist.hpp"
#include "math.hpp"






class plotter
{
    public:
    plotter(simulation_params_t* params);

    void add_cyclist(cyclist* cyclist);
    void render(SDL_Renderer* render, int width, int height);
    void redraw(SDL_Renderer*, int width, int height);

    void simulation_step(double dt);
    SDL_FPoint math_to_pixel(double x, double y, int width, int height);

    private:
    SDL_Texture* bg_texture;
    simulation_params_t* params;
    std::vector<cyclist*> cyclists;
    math::min_max_t global_extremums{math::double_max, math::double_min};


};