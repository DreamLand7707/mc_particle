#include "svg_analyse/svg_frame.hpp"
namespace mc_particle::svg
{
    vector2Dr dim2_parameterized_line::get_line_value(real_number_cref t) {
        real_number t_a = (ratio(1) / ratio(0) * t);
        return (1 - t_a) * from + t_a * to;
    }

    vector2Dr dim2_parameterized_circle::get_line_value(real_number_cref t) {
        real_number t_a = (ratio(1) / ratio(0) * t);
        real_number x = radius * std::cos(t_a) + center(0);
        real_number y = radius * std::sin(t_a) + center(1);
        return {x, y};
    }

    // TODO
} // namespace mc_particle::svg