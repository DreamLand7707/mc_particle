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

    str linear_particle::gradual_color_expand_spiral_sequential_file(
        const particle_parameter_options &opt, const mc_function &target_file,
        const vector3D<real_number> &from, const vector3D<real_number> &to,
        bool clockwise, real_number_cref rad_bias, real_number_cref circles,
        real_number_cref radius, real_number_cref resistence, Input_it iter_begin,
        Input_it iter_end) const {
        particle_normal_options single_particle_opt(opt.rgba, {0, 0, 0}, 1, opt);

        vector3D<real_number> plane_y = to - from;
        auto &&tbe = opt.get_t_begin_end();
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
                    bias1 = tbe(0);

        real_number t = tbe(0), t_ = 0;
        matrix3<real_number> P;
        vector3D<real_number> vel;
        real_number vel_mag = radius * resistence;
        P << plane_x, plane_y, plane_z;

        if (!(target_file.create_fs() && target_file.open()))
            throw std::out_of_range(
                "Can't Create Or Open Function File Correctly!");
        auto &&fout = target_file.get_stream();
        // 从这开始
        for (integer_number it = 0; it < opt.get_particle_num(); ++it) {
            t_ = (t - tbe(0)) / (tbe(1) - tbe(0));
            single_particle_opt.coord = (1 - t_) * from + t_ * to;

            vel(0) = (clockwise ? -1 : 1) * vel_mag *
                     std::sin(fi * (t + bias1) + rad_bias);
            vel(1) = 0;
            vel(2) = vel_mag * std::cos(fi * (t + bias1) + rad_bias);

            vel = P * vel;

            using std::lerp;
            vector4D<real_number> rgba = {1, 1, 1, 1};
            color_position curr = {
                0, {1.0, 1.0, 1.0, 1.0}
            };
            Input_it iter_buff;
            iter_buff = iter_begin;
            if (iter_buff != iter_end) {
                curr = *iter_buff;
                ++iter_buff;
            }
            for (int i = 0; i < 4; i++) {
                if (t < lerp(tbe(0), tbe(1), curr.ratio))
                    rgba(i) = curr.color(i);
            }

            Eigen::Matrix<real_number, 2, 2> A_rgba;
            Eigen::Matrix<real_number, 2, 4> x_rgba, b_rgba;

            for (; iter_buff != iter_end; ++iter_buff) {
                A_rgba << lerp(tbe(0), tbe(1), curr.ratio), 1,
                    lerp(tbe(0), tbe(1), iter_buff->ratio), 1;

                b_rgba << curr.color(0), curr.color(1), curr.color(2),
                    curr.color(3), iter_buff->color(0), iter_buff->color(1),
                    iter_buff->color(2), iter_buff->color(3);

                x_rgba = A_rgba.colPivHouseholderQr().solve(b_rgba);

                for (int i = 0; i < 4; i++) {
                    if (t >= lerp(tbe(0), tbe(1), curr.ratio) &&
                        t < lerp(tbe(0), tbe(1), iter_buff->ratio)) {
                        rgba(i) = t * x_rgba(0, i) + x_rgba(1, i);
                    }
                }

                curr = *iter_buff;
            }
            for (int i = 0; i < 4; i++) {
                if (t >= lerp(tbe(0), tbe(1), curr.ratio)) {
                    rgba(i) = curr.color(i);
                }
            }

            single_particle_opt.rgba = rgba;

            fout << single_particle_opt.format(
                        std::format(
                            "vx=(({:{}.{}f})*({:{}.{}f})^(t-round({:{}.{}f}*1000))*"
                            "(t>=(round({:{}.{}f}*1000))));"
                            "vy=(({:{}.{}f})*({:{}.{}f})^(t-round({:{}.{}f}*1000))*"
                            "(t>=(round({:{}.{}f}*1000))));"
                            "vz=(({:{}.{}f})*({:{}.{}f})^(t-round({:{}.{}f}*1000))*"
                            "(t>=(round({:{}.{}f}*1000))));",
                            vel(0), length, prec, (1 - resistence), length, prec, t_,
                            length, prec, t_, length, prec, vel(1), length, prec,
                            (1 - resistence), length, prec, t_, length, prec, t_,
                            length, prec, vel(2), length, prec, (1 - resistence),
                            length, prec, t_, length, prec, t_, length, prec),
                        length, prec)
                 << '\n';

            t += opt.get_para_counting_period();
        }
        // 到这结束
        return target_file.format();
    }
} // namespace mc_particle