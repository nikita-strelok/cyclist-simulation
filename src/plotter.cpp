#include "plotter.hpp"
#include "math.hpp"

SDL_Texture *create_plot_texture(SDL_Renderer *renderer, SDL_Texture *texture, int width, int height,
                                 std::function<double(double, simulation_params_t)> func,
                                 simulation_params_t params,
                                 double x_min, double x_max,
                                 double y_min, double y_max,
                                 SDL_Color graphColor = {255, 0, 0, 255})
{
    if (!renderer || width <= 0 || height <= 0 || x_min >= x_max || y_min >= y_max)
    {
        return nullptr;
    }
    bool should_clear = false;

    if (!texture)
    {
        should_clear = true;
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    }

    if (!texture)
    {
        return nullptr;
    }

    SDL_SetRenderTarget(renderer, texture);

    if (should_clear)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
    }

    // Устанавливаем цвет графика
    SDL_SetRenderDrawColor(renderer, graphColor.r, graphColor.g, graphColor.b, graphColor.a);

    const double rangeX = x_max - x_min;
    const double rangeY = y_max - y_min;

    // Чтобы избежать деления на ноль
    if (rangeX == 0.0 || rangeY == 0.0)
    {
        SDL_SetRenderTarget(renderer, nullptr);
        return texture; // Возвращаем пустую текстуру
    }

    std::vector<SDL_FPoint> points(width);

    for (int px = 0; px < width; ++px)
    {
        double x = x_min + (static_cast<double>(px) / (width - 1)) * rangeX;
        double y = func(x,params);

        // Нормализация Y с учётом левого нижнего угла как (0,0)
        double normalizedY = (y - y_min) / rangeY;
        float py = height - normalizedY * height;

        // Ограничиваем координаты границами текстуры
        py = std::max(0.0f, std::min(height - 1.0f, py));

        points[px] = {(float)px, py};
    }

    // Рисуем график одной командой
    SDL_RenderLines(renderer, points.data(), static_cast<int>(points.size()));

    // Сбрасываем рендер-таргет
    SDL_SetRenderTarget(renderer, nullptr);

    return texture;
}

plotter::plotter(simulation_params_t *params)
{
    this->params = params;
}

void plotter::add_cyclist(cyclist *cyclist)
{
    cyclists.push_back(cyclist);
}

void plotter::simulation_step(double dt)
{
    for (auto &cyclist : cyclists)
    {
        cyclist->simulation_step(dt);
    }
}

void plotter::render(SDL_Renderer *render, int width, int height)
{
    if (!bg_texture)
        return;
    SDL_RenderTexture(render, bg_texture, nullptr, nullptr);

    for (auto &cyclist : cyclists)
    {
        auto coords = math_to_pixel(cyclist->get_x(), cyclist->get_y(), width, height);
        SDL_FRect rect = {coords.x - 5, coords.y - 5, 10, 10};
        SDL_SetRenderDrawColor(render, cyclist->get_color().r, cyclist->get_color().g, cyclist->get_color().b, cyclist->get_color().a);
        SDL_RenderFillRect(render, &rect);
    }
}

void plotter::redraw(SDL_Renderer *render, int width, int height)
{
    if (!params)
        return;
    SDL_DestroyTexture(bg_texture);
    bg_texture = nullptr;

    for (auto &cyclist : cyclists)
    {
        auto extremums = math::find_min_max(cyclist->func, cyclist->params, 0, params->L);

        if (global_extremums.max < extremums.max)
            global_extremums.max = extremums.max;

        if (global_extremums.min > extremums.min)
            global_extremums.min = extremums.min;
    }
    for (auto &cyclist : cyclists)
    {
        bg_texture = create_plot_texture(render, bg_texture,
                                         width, height,
                                         cyclist->func,
                                         cyclist->params,
                                         0, params->L, global_extremums.min * 1.1, global_extremums.max * 1.1,
                                         cyclist->color);
    }
}

SDL_FPoint plotter::math_to_pixel(double x, double y, int width, int height)
{
    double x_min = 0;
    double x_max = params->L;
    double y_min = global_extremums.min * 1.1;
    double y_max = global_extremums.max * 1.1;

    double normalized_x = (x - x_min) / (x_max - x_min);
    double normalized_y = (y - y_min) / (y_max - y_min);

    float px = static_cast<float>(normalized_x * width);
    float py = static_cast<float>(height - normalized_y * height);

    return {px, py};
}
