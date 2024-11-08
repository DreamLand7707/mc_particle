#pragma once
#include "basic_particle.hpp"
#include "datapack_func_options.hpp"
#include <boost/math/special_functions.hpp>

namespace mc_particle
{

    struct cube_bezier_point;

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

        template <class InputIt>
            requires requires {
                requires std::input_iterator<InputIt>;
                requires std::is_same_v<typename std::iterator_traits<InputIt>::value_type, cube_bezier_point>;
            }
        [[nodiscard]] str solid_color_static_sequential_continuity_points(
            const particle_tick_parameter_options &opt,
            InputIt points_begin, InputIt points_end);
    };

    using empty_cube_bezier_point_t = std::nullopt_t;
    inline constexpr empty_cube_bezier_point_t empty_cube_bezier_point = std::nullopt;

    /**
     * @brief
     *  @ cube bezier
     *  @         1            2            3               4
     *  @        / \          / \          / \             / \
     *  @  (*)left right   left right   left right(!)   left right(*)
     *  ` flag with * is meaning that this dot does not work
     *  ` flag with ! is meaning that this dot is generated by another(left <-> right)
     *  ` if you want to start a bezier, please use:
     *  `cube_bezier_point point1 {<point1>, empty_cube_bezier_point, <ctrl_point>}`
     *  ` if you want to create a point with 2 control points, please use:
     *  `cube_bezier_point point2 {<point1>, <ctrl_point1>, <ctrl_point2>}`
     *  ` if you want to create a smooth point with only 1 control point, please use:
     *  `cube_bezier_point point2 {<point1>, <ctrl_point1>}`
     */

    struct cube_bezier_point {
        vector3D<real_number> point;
        std::optional<vector3D<real_number>> left = empty_cube_bezier_point;
        std::optional<vector3D<real_number>> right = empty_cube_bezier_point;
    };

    class cube_bezier_integral_polynomial {
     public:
        real_number coff[5] {};

     public:
        explicit cube_bezier_integral_polynomial(
            const Eigen::Matrix<real_number, 4, 3> &points) {
            Eigen::Matrix4<real_number> conv_mat {
                {-1,  3, -3, 1},
                { 3, -6,  3, 0},
                {-3,  3,  0, 0},
                { 1,  0,  0, 0}
            };
            Eigen::Matrix<real_number, Eigen::Dynamic, Eigen::Dynamic> raw_coff = conv_mat * points;
            coff[0] = 9 * (std::pow(raw_coff(0, 0), 2) +
                           std::pow(raw_coff(0, 1), 2) +
                           std::pow(raw_coff(0, 2), 2));
            coff[1] = 12 * (raw_coff(0, 0) * raw_coff(1, 0) +
                            raw_coff(0, 1) * raw_coff(1, 1) +
                            raw_coff(0, 2) * raw_coff(1, 2));
            coff[2] = 6 * (raw_coff(0, 0) * raw_coff(2, 0) +
                           raw_coff(0, 1) * raw_coff(2, 1) +
                           raw_coff(0, 2) * raw_coff(2, 2)) +
                      4 * (std::pow(raw_coff(1, 0), 2) +
                           std::pow(raw_coff(1, 1), 2) +
                           std::pow(raw_coff(1, 2), 2));
            coff[3] = 4 * (raw_coff(1, 0) * raw_coff(2, 0) +
                           raw_coff(1, 1) * raw_coff(2, 1) +
                           raw_coff(1, 2) * raw_coff(2, 2));
            coff[4] = 1 * (std::pow(raw_coff(2, 0), 2) +
                           std::pow(raw_coff(2, 1), 2) +
                           std::pow(raw_coff(2, 2), 2));
        }

        real_number operator()(real_number_cref t) const {
            return sqrt(pow(t, 4) * coff[0] +
                        pow(t, 3) * coff[1] +
                        pow(t, 2) * coff[2] +
                        t * coff[3] +
                        coff[4]);
        }
    };

    real_number legendre_d(integer_number_cref l, real_number_cref x);
    real_number find_legendre_root_at(integer_number_cref n, real_number_cref initial_guess, real_number_cref tolerance, integer_number_cref max_iterations);
    Eigen::VectorX<real_number> get_legendre_root(integer_number_cref l, real_number_cref tolerance, integer_number_cref max_iterations);
    template <unsigned int level, std::invocable<real_number> invt>
    real_number gauss_legender_integral(const invt &func, real_number_cref from, real_number_cref to);


} // namespace mc_particle

#include "cube_bezier_particle/solid_color_static_sequential_continuity_points_impl.hpp"
#include "cube_bezier_particle/gauss_legender_integral_impl.hpp"