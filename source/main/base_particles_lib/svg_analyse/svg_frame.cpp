#include "svg_analyse/svg_frame.hpp"
#include "basic_particle.hpp"
namespace mc_particle::svg
{
    vector2Dr dim2_parameterized_line::get_line_value(real_number_cref t) const {
        real_number t_a = (ratio(1) / ratio(0) * t);
        return (1 - t_a) * from + t_a * to;
    }

    vector2Dr dim2_parameterized_circle::get_line_value(real_number_cref t) const {
        real_number t_a = (ratio(1) / ratio(0) * t);
        real_number x = radius * std::cos(t_a) + center(0);
        real_number y = radius * std::sin(t_a) + center(1);
        return {x, y};
    }

    vector2Dr dim2_parameterized_ellipse::get_line_value(real_number_cref t) const {
        real_number t_a = (ratio(1) / ratio(0) * t);
        real_number x = half_axs_length(0) * std::cos(t_a) + center(0);
        real_number y = half_axs_length(1) * std::sin(t_a) + center(1);
        return {x, y};
    }

    vector2Dr dim2_parameterized_arc::get_line_value(real_number_cref t) const {
        real_number t_a = (ratio(1) / ratio(0) * t);
        real_number x = radius * std::cos(std::fmod(t_a, angle_from_to(1) - angle_from_to(0)) + angle_from_to(0)) + center(0);
        real_number y = radius * std::sin(std::fmod(t_a, angle_from_to(1) - angle_from_to(0)) + angle_from_to(0)) + center(0);
        return {x, y};
    }

    vector2Dr dim2_parameterized_elliarc::get_line_value(real_number t) const {
        real_number t_a = (ratio(1) / ratio(0) * t);
        real_number x = half_axs_length(0) * std::cos(std::fmod(t_a, angle_from_to(1) - angle_from_to(0)) + angle_from_to(0)) + center(0);
        real_number y = half_axs_length(1) * std::sin(std::fmod(t_a, angle_from_to(1) - angle_from_to(0)) + angle_from_to(0)) + center(1);
        return {x, y};
    }

    vector2Dr dim2_parameterized_quad_bezier::get_line_value(real_number_cref t) const {
        real_number t_a = (ratio(1) / ratio(0)) * t;
        real_number u = 1.0 - t_a;

        if (t_a > 1.0)
            t_a = 1.0;
        else if (t_a < 0.0)
            t_a = 0.0;

        real_number x = u * u * from.value()(0) + 2 * u * t_a * ctrl.value()(0) + t_a * t_a * to(0);
        real_number y = u * u * from.value()(1) + 2 * u * t_a * ctrl.value()(1) + t_a * t_a * to(1);

        return vector2Dr {x, y};
    }

    vector2Dr dim2_parameterized_cube_bezier::get_line_value(real_number_cref t) const {
        real_number t_a = (ratio(1) / ratio(0)) * t;
        real_number u = 1 - t_a;

        if (t_a > 1.0)
            t_a = 1.0;
        else if (t_a < 0.0)
            t_a = 0.0;

        vector2Dr point = (u * u * u) * from.value() + (3 * u * u * t_a) * ctrl_f.value() + (3 * u * t_a * t_a) * ctrl_t + (t_a * t_a * t_a) * to;

        return point;
    }

    // TODO
} // namespace mc_particle::svg