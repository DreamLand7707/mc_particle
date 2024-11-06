#pragma once

#include "basic_particle.hpp"
#include "datapack_func_options.hpp"


namespace mc_particle
{
    struct color_position {
        real_number ratio;
        vector4D<real_number> color;
    };

    class linear_particle {
     public:
        real_number error = 1e-6;
        integer_number length = 0;
        integer_number prec = 6;

     public:
        [[nodiscard]] str solid_color_static_sequential(
            const particle_tick_parameter_options &opt,
            const vector3D<real_number> &from, const vector3D<real_number> &to) const;
        [[nodiscard]] str solid_color_static_instantaneous(
            const particle_parameter_options &opt,
            const vector3D<real_number> &from, const vector3D<real_number> &to) const;

        template <class Input_it>
            requires requires () {
                requires std::input_iterator<Input_it>;
                requires std::is_same_v<color_position, typename std::iterator_traits<Input_it>::value_type>;
            }
        [[nodiscard]] str gradual_color_static_sequential(
            const particle_rgba_tick_parameter_options &opt,
            const vector3D<real_number> &from, const vector3D<real_number> &to,
            Input_it iter_begin, Input_it iter_end) const;

        [[nodiscard]] str gradual_color_static_sequential(
            const particle_rgba_tick_parameter_options &opt,
            const vector3D<real_number> &from, const vector3D<real_number> &to,
            std::initializer_list<color_position> list) const;

        [[nodiscard]] str solid_color_static_spiral_sequential(
            const particle_tick_parameter_options &opt,
            const vector3D<real_number> &from, const vector3D<real_number> &to,
            bool clockwise,
            real_number_cref rad_bias,
            real_number_cref circles,
            real_number_cref radius) const;

        [[nodiscard]] str solid_color_expand_spiral_instantaneous_file(
            const particle_parameter_options &opt,
            const mc_function &target_file,
            const vector3D<real_number> &from,
            const vector3D<real_number> &to,
            bool clockwise,
            real_number_cref rad_bias,
            real_number_cref circles,
            real_number_cref radius,
            real_number_cref resistence) const;
    };
} // namespace mc_particle


#include "linear_particle/gradual_color_static_sequential_impl.hpp"
