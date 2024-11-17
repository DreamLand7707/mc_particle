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
        const particle_tick_parameter_options &opt, const mc_function &target_file,
        InputIt points_begin, InputIt points_end, bool closed) {
        real_number total = 0;
        std::vector<real_number> ratio_per_section;
        cube_bezier_integral_polynomial poly;
        Eigen::Matrix<real_number, 3, 4> points;
        InputIt rightp = points_begin, leftp;
        vector3D<real_number> ctrl1, ctrl2;
        while (true) {
            leftp = rightp++;
            if (rightp == points_end) {
                if (closed)
                    rightp = points_begin;
                else
                    break;
            }
            ctrl1 = leftp->right.value_or(leftp->left.has_value()
                                              ? (leftp->point + leftp->point - leftp->left.value())
                                              : (leftp->point));
            ctrl2 = rightp->left.value_or(rightp->right.has_value()
                                              ? (rightp->point + rightp->point - rightp->right.value())
                                              : (rightp->point));
            points << leftp->point, ctrl1, ctrl2, rightp->point;
            poly.reassign(points);
            ratio_per_section.push_back(gauss_legender_integral<9>(poly, 0, 1));
            total += ratio_per_section.back();
            if (rightp == points_begin)
                break;
        }
        if (ratio_per_section.empty())
            return "";
        for (auto &x : ratio_per_section)
            x /= total;

        integer_number particle_num = opt.get_particle_num(), generate_cnt = 0, file_index = 0;
        auto &&t_begin_end = opt.get_t_begin_end();
        int section = 0;
        real_number c_begin = t_begin_end(0),
                    c_end = c_begin + (t_begin_end(1) - t_begin_end(0)) * ratio_per_section[section],
                    c_val = c_begin,
                    section_ratio = 0;
        vector3D<real_number> draw_point {};
        rightp = points_begin;
        leftp = rightp++;
        if (rightp == points_end && closed) {
            rightp = points_begin;
        }
        ctrl1 = leftp->right.value_or(leftp->left.has_value()
                                          ? (leftp->point + leftp->point - leftp->left.value())
                                          : (leftp->point));
        ctrl2 = rightp->left.value_or(rightp->right.has_value()
                                          ? (rightp->point + rightp->point - rightp->right.value())
                                          : (rightp->point));
        points << leftp->point, ctrl1, ctrl2, rightp->point;
        cube_bezier_polynomial bezier_ploy(points);
        if (!target_file.delete_all_temp_file())
            std::cerr << "Can't delete temp file!" << std::endl;
        mc_function curr_func = target_file.create_temp_file(std::format("temp-{}", file_index));

        if (!(curr_func.create_fs() && curr_func.open()))
            throw std::runtime_error("Can't Create Or Open Function File Correctly!");
        if (!(target_file.create_fs() && target_file.open()))
            throw std::runtime_error("Can't Create Or Open Function File Correctly!");

        bool have_writen = false;

        particle_normal_options single_particle_opt(opt.rgba, {0, 0, 0}, 1, opt);
        for (integer_number i = 0; i < particle_num; i++) {
            if (c_val > c_end) {
                if (rightp == points_begin)
                    break;
                section++;
                c_begin = c_end;
                c_end += (t_begin_end(1) - t_begin_end(0)) * ratio_per_section[section];
                leftp = rightp++;
                if (rightp == points_end) {
                    if (closed)
                        rightp = points_begin;
                    else
                        break;
                }
                ctrl1 = leftp->right.value_or(leftp->left.has_value()
                                                  ? (leftp->point + leftp->point - leftp->left.value())
                                                  : (leftp->point));
                ctrl2 = rightp->left.value_or(rightp->right.has_value()
                                                  ? (rightp->point + rightp->point - rightp->right.value())
                                                  : (rightp->point));
                points << leftp->point, ctrl1, ctrl2, rightp->point;
                bezier_ploy.reassign(points);
            }
            if (generate_cnt >= opt.gent_counting_per_tick) {
                target_file << curr_func.format(file_index, mc_function::APPEND) << '\n';
                curr_func.close();
                file_index++;
                curr_func = target_file.create_temp_file(std::format("temp-{}", file_index));
                generate_cnt = 0;
                if (!(curr_func.create_fs() && curr_func.open()))
                    throw std::runtime_error("Can't Create Or Open Function File Correctly!");
                have_writen = false;
            }
            section_ratio = (c_val - c_begin) / (c_end - c_begin);
            draw_point = bezier_ploy(section_ratio);
            single_particle_opt.coord = draw_point;
            curr_func << single_particle_opt.format("null", length, prec) << '\n';
            have_writen = true;
            c_val += opt.get_para_counting_period();
            generate_cnt++;
        }
        if (curr_func.is_open() && have_writen) {
            target_file << curr_func.format(file_index, mc_function::APPEND) << '\n';
            curr_func.close();
        }
        target_file.close();
        return target_file.format();
    }
} // namespace mc_particle