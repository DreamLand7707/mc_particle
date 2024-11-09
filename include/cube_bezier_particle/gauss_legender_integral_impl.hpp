
#pragma once

#include "../cube_bezier_particle.hpp"

namespace mc_particle
{
    template <unsigned int level, std::invocable<real_number> invt>
    real_number gauss_legender_integral(const invt &func, real_number_cref from, real_number_cref to,
                                        real_number_cref caldetail_error, integer_number_cref caldetail_times) {
        static Eigen::Matrix<real_number, -1, -1> legender_root = get_legendre_root(level, caldetail_error, caldetail_times), Sx;
        static bool have = false;
        if (!have) {
            Eigen::Matrix<real_number, -1, -1> Sa(level, level), Sb(level, 1);
            for (int i = 0; i < level; i++)
                Sa.row(i) << legender_root.transpose().array().pow(i).matrix();
            for (int i = 1; i <= level; i++)
                Sb(i - 1) = (1.0 / i) * (pow(1, i) - pow(-1, i));
            Sx = Sa.householderQr().solve(Sb);
            have = true;
        }
        real_number res = 0, ratio = (to - from) / 2, offset = (to + from) / 2;
        for (int i = 0; i < level; i++)
            res += Sx(i) * func(ratio * legender_root(i) + offset);
        res *= ratio;
        return res;
    }
} // namespace mc_particle