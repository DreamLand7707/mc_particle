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
    (float_spec {(X), (length), (prec)})
        auto sh = WARP(mk_shift), ro = WARP(mk_ratio);
        for (int i = 0; i < 3; i++) {
            ssin << ((i == 0) ? ("x = (") : (i == 1 ? ("y = (") : ("z = (")));
            ssin << std::format("("
                                "   ((1 - ({} + {} * t)) ^ 3)"
                                "   *"
                                "   {}"
                                ")"
                                "+"
                                "("
                                "   ((1 - ({} + {} * t)) ^ 2)"
                                "   *"
                                "   ({} + {} * t)"
                                "   *"
                                "   {}"
                                ")"
                                "+"
                                "("
                                "   (1 - ({} + {} * t))"
                                "   *"
                                "   (({} + {} * t) ^ 2)"
                                "   *"
                                "   {}"
                                ")"
                                "+"
                                "("
                                "   (({} + {} * t) ^ 3)"
                                "   *"
                                "   {}"
                                ")",
                                sh, ro, WARP(from(i)),
                                sh, ro, sh, ro, WARP(ctrl_point_1(i) * 3),
                                sh, ro, sh, ro, WARP(ctrl_point_2(i) * 3),
                                sh, ro, WARP(to(i)));
            ssin << "); ";
        }
#undef WARP
        return opt.format(ssin.str(), "null", length, prec);
    }

    real_number legendre_d(integer_number_cref l, real_number_cref x) {
        if (l == 0)
            return 0.0;
        if (l == 1)
            return 1.0;
        return l * ((boost::math::legendre_p(l - 1, x) - x * boost::math::legendre_p(l, x)) / (1 - x * x));
    }

    real_number find_legendre_root_at(integer_number_cref n,
                                      real_number_cref initial_guess,
                                      real_number_cref tolerance,
                                      integer_number_cref max_iterations) {
        real_number x = initial_guess;
        for (int i = 0; i < max_iterations; ++i) {
            real_number Pn = boost::math::legendre_p(n, x);
            real_number Pn_prime = legendre_d(n, x);
            real_number x_new = x - Pn / Pn_prime;

            if (std::abs(x_new - x) < tolerance) {
                return x_new;
            }

            x = x_new;
        }

        return x;
    }

    Eigen::VectorX<real_number> get_legendre_root(integer_number_cref l, real_number_cref tolerance, integer_number_cref max_iterations) {
        integer_number num_roots = l;
        Eigen::VectorXd ret(l);
        for (integer_number i = 1; i <= num_roots; ++i) {
            real_number initial_guess = std::cos(std::numbers::pi * (4 * i - 1) / (4 * num_roots + 2));
            real_number root = find_legendre_root_at(num_roots, initial_guess, tolerance, max_iterations);
            ret(i - 1) = root;
        }
        std::sort(ret.begin(), ret.end());
        return ret;
    }
} // namespace mc_particle