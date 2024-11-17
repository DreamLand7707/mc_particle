#pragma once

#include "../cube_bezier_particle.hpp"

namespace mc_particle
{
    template <class InputIt>
        requires requires {
            requires std::input_iterator<InputIt>;
            requires std::is_same_v<typename std::iterator_traits<InputIt>::value_type, cube_bezier_point>;
        }
    str cube_bezier_particle::solid_color_static_sequential_continuity_points(
        const particle_tick_parameter_options &opt,
        InputIt points_begin, InputIt points_end) {
    }
} // namespace mc_particle