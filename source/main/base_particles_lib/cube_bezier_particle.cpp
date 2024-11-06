#include "cube_bezier_particle.hpp"

namespace mc_particle
{
    str cube_bezier_particle::solid_color_static_sequential(
        const particle_tick_parameter_options &opt,
        const vector3D<real_number> &from, const vector3D<real_number> &to,
        const vector3D<real_number> &ctrl_point_1, const vector3D<real_number> &ctrl_point_2) const {
        str_stream ssin;
        real_number mk_ratio, mk_shift;
        auto &tbe = opt.get_t_begin_end();
        mk_ratio = 1.0 / (tbe(1) - tbe(0));
        mk_shift = -(tbe(0) * mk_ratio);
        // bezier curve function: (1-t)^3 * A + t*(1-t)^2 * B + t^2*(1-t) * C + t^3 * D
#define WARP(X) \
    float_spec { X, length, prec }
        auto sh = WARP(mk_shift), ro = WARP(mk_ratio);
        for (int i = 0; i < 3; i++) {
            ssin << ((i == 0) ? ("x = (") : (i == 1 ? ("y = (") : ("z = (")));
            ssin << std::format("(((1 - ({} + {} * t)) ^ 3) * {}) + "
                                "((({} + {} * t) * (1 - ({} + {} * t)) ^ 2) * {}) + "
                                "((({} + {} * t) ^ 2 * (1 - ({} + {} * t))) * {}) + "
                                "((({} + {} * t) ^ 3) * {})",
                                sh, ro, WARP(from(i)),
                                sh, ro, sh, ro, WARP(ctrl_point_1(i)),
                                sh, ro, sh, ro, WARP(ctrl_point_2(i)),
                                sh, ro, WARP(to(i)));
        }
#undef WARP
        return opt.format(ssin.str(), "null", length, prec);
    }
} // namespace mc_particle