#pragma once

#include "../linear_particle.hpp"

namespace mc_particle
{
    template <class Input_it>
        requires requires () {
            requires std::input_iterator<Input_it>;
            requires std::is_same_v<color_position, typename std::iterator_traits<
                                                        Input_it>::value_type>;
        }
    str linear_particle::gradual_color_static_spiral_sequential(
        const particle_rgba_tick_parameter_options &opt,
        const vector3D<real_number> &from, const vector3D<real_number> &to,
        bool clockwise, real_number_cref rad_bias, real_number_cref circles,
        real_number_cref radius, Input_it iter_begin, Input_it iter_end) const {
        using std::lerp;
        vector3D<real_number> plane_y = to - from;
        real_number length_ft = plane_y.norm();
        auto &&tbe = opt.get_t_begin_end();

        matrix2<real_number> solve_A;
        vector2D<real_number> solve_x, solve_b;
        solve_A << tbe(0), 1, tbe(1), 1;
        solve_b << 0, length_ft;
        solve_x = solve_A.colPivHouseholderQr().solve(solve_b);

        plane_y.normalize();
        vector3D<real_number> plane_x, plane_z;
        if (std::abs(plane_y(1)) < error)
            plane_x = {0, 1, 0};
        else {
            if (hypot(plane_y(0), plane_y(2)) / std::abs(plane_y(1)) < error)
                plane_x = {1, 0, 0};
            else
                plane_x = {0, 1, 0};
            plane_x = plane_x - (plane_x.dot(plane_y) * plane_y);
            plane_x.normalize();
        }
        plane_z = plane_y.cross(plane_x);

        real_number fi = 2.0 * std::numbers::pi_v<real_number> * circles /
                         (tbe(1) - tbe(0)),
                    bias1 = tbe(0); // 每一t移动了多少角度

        str_stream ssin[4];
        ssin[0] << std::format(
            "(({}({:{}.{}f}))*(sin(({:{}.{}f})*(t+({:{}.{}f}))+({:{}.{}f}))))",
            clockwise ? "-" : "", radius, length, prec, fi, length, prec, bias1,
            length, prec, rad_bias, length, prec);
        ssin[1] << std::format("(({:{}.{}f})*t+({:{}.{}f}))", solve_x(0), length,
                               prec, solve_x(1), length, prec);
        ssin[2] << std::format(
            "(({:{}.{}f})*(cos(({:{}.{}f})*(t+({:{}.{}f}))+({:{}.{}f}))))", radius,
            length, prec, fi, length, prec, bias1, length, prec, rad_bias, length,
            prec);

        ssin[3] << std::format(
            " x = "
            "({:{}.{}f})*({})+"
            "({:{}.{}f})*({})+"
            "({:{}.{}f})*({})+"
            "({:{}.{}f});"
            " y = "
            "({:{}.{}f})*({})+"
            "({:{}.{}f})*({})+"
            "({:{}.{}f})*({})+"
            "({:{}.{}f});"
            " z = "
            "({:{}.{}f})*({})+"
            "({:{}.{}f})*({})+"
            "({:{}.{}f})*({})+"
            "({:{}.{}f});",
            plane_x(0), length, prec, ssin[0].str(), plane_y(0), length, prec,
            ssin[1].str(), plane_z(0), length, prec, ssin[2].str(), from(0), length,
            prec,

            plane_x(1), length, prec, ssin[0].str(), plane_y(1), length, prec,
            ssin[1].str(), plane_z(1), length, prec, ssin[2].str(), from(1), length,
            prec,

            plane_x(2), length, prec, ssin[0].str(), plane_y(2), length, prec,
            ssin[1].str(), plane_z(2), length, prec, ssin[2].str(), from(2), length,
            prec);

        str_stream ssin_rgba[4];
        ssin_rgba[0] << " cr = ";
        ssin_rgba[1] << " cg = ";
        ssin_rgba[2] << " cb = ";
        ssin_rgba[3] << " alpha = ";
        color_position curr = {
            0, {1.0, 1.0, 1.0, 1.0}
        };
        if (iter_begin != iter_end) {
            curr = *iter_begin;
            ++iter_begin;
        }
        for (int i = 0; i < 4; i++) {
            ssin_rgba[i] << std::format("((t<({:{}.{}f}))*({:{}.{}f}))",
                                        lerp(tbe(0), tbe(1), curr.ratio), length,
                                        prec, curr.color(i), length, prec);
        }

        Eigen::Matrix<real_number, 2, 2> A_rgba;
        Eigen::Matrix<real_number, 2, 4> x_rgba, b_rgba;

        for (; iter_begin != iter_end; ++iter_begin) {
            A_rgba << lerp(tbe(0), tbe(1), curr.ratio), 1,
                lerp(tbe(0), tbe(1), iter_begin->ratio), 1;

            b_rgba << curr.color(0), curr.color(1), curr.color(2), curr.color(3),
                iter_begin->color(0), iter_begin->color(1), iter_begin->color(2),
                iter_begin->color(3);

            x_rgba = A_rgba.colPivHouseholderQr().solve(b_rgba);

            for (int i = 0; i < 4; i++) {
                ssin_rgba[i] << std::format(
                    " + "
                    "(((t>=({:{}.{}f}))&(t<({:{}.{}f})))*(({:{}.{}f})*t+({:{}.{}f})"
                    "))",
                    lerp(tbe(0), tbe(1), curr.ratio), length, prec,
                    lerp(tbe(0), tbe(1), iter_begin->ratio), length, prec,
                    x_rgba(0, i), length, prec, x_rgba(1, i), length, prec);
            }

            curr = *iter_begin;
        }
        for (int i = 0; i < 4; i++) {
            ssin_rgba[i] << std::format(" + ((t>=({:{}.{}f}))*({:{}.{}f}))",
                                        lerp(tbe(0), tbe(1), curr.ratio), length,
                                        prec, curr.color(i), length, prec);
            ssin[3] << ssin_rgba[i].str() << "; ";
        }

        return opt.format(ssin[3].str(), "null", length, prec);
    }
} // namespace mc_particle