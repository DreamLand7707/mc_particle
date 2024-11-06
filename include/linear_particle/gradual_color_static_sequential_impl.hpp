#pragma once

#include "../linear_particle.hpp"

namespace mc_particle
{
    template <class Input_it>
        requires requires () {
            requires std::input_iterator<Input_it>;
            requires std::is_same_v<color_position, typename std::iterator_traits<Input_it>::value_type>;
        }
    str linear_particle::gradual_color_static_sequential(
        const particle_rgba_tick_parameter_options &opt,
        const vector3D<real_number> &from, const vector3D<real_number> &to,
        Input_it iter_begin, Input_it iter_end) const {
        using std::lerp;
        Eigen::Matrix<real_number, 2, 2> A;
        Eigen::Matrix<real_number, 2, 3> x, b;
        const auto &tbe = opt.get_t_begin_end();
        A << 1, tbe(0),
            1, tbe(1);
        b << from(0), from(1), from(2),
            to(0), to(1), to(2);
        x = A.colPivHouseholderQr().solve(b);

        str_stream ssin;
        ssin << std::format(" x = ({:{}.{}f}) + ({:{}.{}f}) * t; ", x(0, 0), length, prec, x(1, 0), length, prec)
             << std::format(" y = ({:{}.{}f}) + ({:{}.{}f}) * t; ", x(0, 1), length, prec, x(1, 1), length, prec)
             << std::format(" z = ({:{}.{}f}) + ({:{}.{}f}) * t; ", x(0, 2), length, prec, x(1, 2), length, prec);

        str_stream ssin_rgba[4];
        ssin_rgba[0] << " cr = ";
        ssin_rgba[1] << " cg = ";
        ssin_rgba[2] << " cb = ";
        ssin_rgba[3] << " alpha = ";
        color_position curr = {0, {1.0, 1.0, 1.0, 1.0}};
        if (iter_begin != iter_end) {
            curr = *iter_begin;
            ++iter_begin;
        }
        for (int i = 0; i < 4; i++) {
            ssin_rgba[i] << std::format("((t<({:{}.{}f}))*({:{}.{}f}))",
                                        lerp(tbe(0), tbe(1), curr.ratio), length, prec,
                                        curr.color(i), length, prec);
        }

        Eigen::Matrix<real_number, 2, 2> A_rgba;
        Eigen::Matrix<real_number, 2, 4> x_rgba, b_rgba;

        for (; iter_begin != iter_end; ++iter_begin) {
            A_rgba << lerp(tbe(0), tbe(1), curr.ratio), 1,
                lerp(tbe(0), tbe(1), iter_begin->ratio), 1;

            b_rgba << curr.color(0), curr.color(1), curr.color(2), curr.color(3),
                iter_begin->color(0), iter_begin->color(1), iter_begin->color(2), iter_begin->color(3);

            x_rgba = A_rgba.colPivHouseholderQr().solve(b_rgba);

            for (int i = 0; i < 4; i++) {
                ssin_rgba[i] << std::format(" + (((t>=({:{}.{}f}))&(t<({:{}.{}f})))*(({:{}.{}f})*t+({:{}.{}f})))",
                                            lerp(tbe(0), tbe(1), curr.ratio), length, prec,
                                            lerp(tbe(0), tbe(1), iter_begin->ratio), length, prec,
                                            x_rgba(0, i), length, prec,
                                            x_rgba(1, i), length, prec);
            }

            curr = *iter_begin;
        }
        for (int i = 0; i < 4; i++) {
            ssin_rgba[i] << std::format(" + ((t>=({:{}.{}f}))*({:{}.{}f}))",
                                        lerp(tbe(0), tbe(1), curr.ratio), length, prec,
                                        curr.color(i), length, prec);
            ssin << ssin_rgba[i].str() << "; ";
        }

        return opt.format(ssin.str(), "null", length, prec);
    }
} // namespace mc_particle