#include "linear_particle.hpp"

namespace mc_particle
{

    str linear_particle::solid_color_static_sequential(
        const particle_tick_parameter_options &opt,
        const vector3D<real_number> &from, const vector3D<real_number> &to) const {
        matrix<real_number, 2, 2> A;
        matrix<real_number, 2, 3> x, b;
        const auto &tbe = opt.get_t_begin_end();
        A << 1, tbe(0),
            1, tbe(1);
        b << from(0), from(1), from(2),
            to(0), to(1), to(2);
        x = A.colPivHouseholderQr().solve(b);

        str_stream ssin;
        ssin << std::format("x = ({:{}.{}f}) + ({:{}.{}f}) * t; ", x(0, 0), length, prec, x(1, 0), length, prec)
             << std::format("y = ({:{}.{}f}) + ({:{}.{}f}) * t; ", x(0, 1), length, prec, x(1, 1), length, prec)
             << std::format("z = ({:{}.{}f}) + ({:{}.{}f}) * t; ", x(0, 2), length, prec, x(1, 2), length, prec);

        return opt.format(ssin.str(), "null", length, prec);
    }

    str linear_particle::solid_color_static_instantaneous(
        const particle_parameter_options &opt,
        const vector3D<real_number> &from, const vector3D<real_number> &to) const {
        matrix<real_number, 2, 2> A;
        matrix<real_number, 2, 3> x, b;
        const auto &tbe = opt.get_t_begin_end();
        A << 1, tbe(0),
            1, tbe(1);
        b << from(0), from(1), from(2),
            to(0), to(1), to(2);
        x = A.colPivHouseholderQr().solve(b);

        str_stream ssin;
        ssin << std::format("x = ({:{}.{}f}) + ({:{}.{}f}) * t; ", x(0, 0), length, prec, x(1, 0), length, prec)
             << std::format("y = ({:{}.{}f}) + ({:{}.{}f}) * t; ", x(0, 1), length, prec, x(1, 1), length, prec)
             << std::format("z = ({:{}.{}f}) + ({:{}.{}f}) * t; ", x(0, 2), length, prec, x(1, 2), length, prec);

        return opt.format(ssin.str(), "null", length, prec);
    }

    str linear_particle::gradual_color_static_sequential(const particle_rgba_tick_parameter_options &opt,
                                                         const vector3D<real_number> &from, const vector3D<real_number> &to,
                                                         std::initializer_list<color_position> list) const {
        return gradual_color_static_sequential(opt, from, to, list.begin(), list.end());
    }

    str linear_particle::gradual_color_static_instantaneous(
        const particle_rgba_parameter_options &opt,
        const vector3D<real_number> &from, const vector3D<real_number> &to,
        std::initializer_list<color_position> list) const {
        return gradual_color_static_instantaneous(opt, from, to, list.begin(),
                                                  list.end());
    }

    str linear_particle::solid_color_static_spiral_sequential(const particle_tick_parameter_options &opt,
                                                              const vector3D<real_number> &from,
                                                              const vector3D<real_number> &to,
                                                              bool clockwise,
                                                              real_number_cref rad_bias,
                                                              real_number_cref circles,
                                                              real_number_cref radius) const {
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
                    bias1 = tbe(0);  // 每一t移动了多少角度

        str_stream ssin[4];
        ssin[0] << std::format(
            "(({}({:{}.{}f}))*(sin(({:{}.{}f})*(t+({:{}.{}f}))+({:{}.{}f}))))",
            clockwise ? "-" : "", radius, length, prec, fi, length, prec, bias1,
            length, prec, rad_bias, length, prec);
        ssin[1] << std::format("(({:{}.{}f})*t+({:{}.{}f}))", solve_x(0),
                               length, prec, solve_x(1), length, prec);
        ssin[2] << std::format(
            "(({:{}.{}f})*(cos(({:{}.{}f})*(t+({:{}.{}f}))+({:{}.{}f}))))",
            radius, length, prec, fi, length, prec, bias1, length, prec,
            rad_bias, length, prec);

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
            ssin[1].str(), plane_z(0), length, prec, ssin[2].str(), from(0),
            length, prec,

            plane_x(1), length, prec, ssin[0].str(), plane_y(1), length, prec,
            ssin[1].str(), plane_z(1), length, prec, ssin[2].str(), from(1),
            length, prec,

            plane_x(2), length, prec, ssin[0].str(), plane_y(2), length, prec,
            ssin[1].str(), plane_z(2), length, prec, ssin[2].str(), from(2),
            length, prec);

        return opt.format(ssin[3].str(), "null", length, prec);
    }

    

    str linear_particle::solid_color_expand_spiral_instantaneous_file(const particle_parameter_options &opt,
                                                                      const mc_function &target_file,
                                                                      const vector3D<real_number> &from,
                                                                      const vector3D<real_number> &to,
                                                                      bool clockwise,
                                                                      real_number_cref rad_bias,
                                                                      real_number_cref circles,
                                                                      real_number_cref radius,
                                                                      real_number_cref resistence) const {
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
        real_number fi = 2.0 * std::numbers::pi_v<real_number> * circles / (tbe(1) - tbe(0)),
                    bias1 = tbe(0);

        real_number t = tbe(0), t_ = 0;
        matrix3<real_number> P;
        vector3D<real_number> vel;
        real_number vel_mag = radius * resistence;
        P << plane_x, plane_y, plane_z;

        if (!(target_file.create_fs() && target_file.open()))
            throw std::out_of_range("Can't Create Or Open Function File Correctly!");
        auto &&fout = target_file.get_stream();
        for (integer_number it = 0; it < opt.get_particle_num(); ++it) {
            t_ = (t - tbe(0)) / (tbe(1) - tbe(0));
            single_particle_opt.coord = (1 - t_) * from + t_ * to;

            vel(0) = (clockwise ? -1 : 1) * vel_mag * std::sin(fi * (t + bias1) + rad_bias);
            vel(1) = 0;
            vel(2) = vel_mag * std::cos(fi * (t + bias1) + rad_bias);

            vel = P * vel;

            fout << single_particle_opt.format(
                        std::format("vx=(({:{}.{}f})*({:{}.{}f})^t);"
                                    "vy=(({:{}.{}f})*({:{}.{}f})^t);"
                                    "vz=(({:{}.{}f})*({:{}.{}f})^t);",
                                    vel(0), length, prec, (1 - resistence), length, prec,
                                    vel(1), length, prec, (1 - resistence), length, prec,
                                    vel(2), length, prec, (1 - resistence), length, prec),
                        length, prec)
                 << '\n';

            t += opt.get_para_counting_period();
        }
        return target_file.format();
    }
} // namespace mc_particle