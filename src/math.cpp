#include "math.hpp"

namespace math
{

    min_max_t find_min_max(std::function<double(double,simulation_params_t)> func, simulation_params_t params, double x_min, double x_max, double step)
    {
        double f_min = double_max;
        double f_max = double_min;

        for (double x = x_min; x <= x_max; x += step)
        {
            double y = func(x, params);
            if (y < f_min)
                f_min = y;
            if (y > f_max)
                f_max = y;
        }

        return {f_min, f_max};
    }

}