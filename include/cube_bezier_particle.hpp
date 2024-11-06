#pragma once
#include "basic_particle.hpp"
#include "datapack_func_options.hpp"

namespace mc_particle
{
    class cube_bezier_particle {
     public:
        real_number error = 1e-6;
        integer_number length = 0;
        integer_number prec = 6;

     public:
        [[nodiscard]] str solid_color_static_sequential(
            const particle_tick_parameter_options &opt,
            const vector3D<real_number> &from, const vector3D<real_number> &to,
            const vector3D<real_number> &ctrl_point_1, const vector3D<real_number> &ctrl_point_2) const;
    };
} // namespace mc_particle