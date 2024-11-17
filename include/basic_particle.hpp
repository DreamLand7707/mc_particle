#pragma once

#include <cln/cln.h>

#include <ginac/ginac.h>

#include <Eigen/Eigen>

#include <boost/format.hpp>

#include <string>
#include <format>
#include <sstream>
#include <fstream>
#include <functional>
#include <regex>
#include <locale>

#include <cstring>
#include <cmath>
#include <cstdint>
#include <numbers>
#include <numeric>

#include "config_particle_lib.hpp"

#include <type_traits>
#include <concepts>
#include <iterator>

// data structure
#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <optional>


namespace mc_particle
{
    template <class T>
    using point2D = Eigen::Vector2<T>;
    template <class T>
    using point3D = Eigen::Vector3<T>;
    template <class T>
    using point4D = Eigen::Vector4<T>;

    template <class T>
    using vector2D = Eigen::Vector2<T>;
    template <class T>
    using vector3D = Eigen::Vector3<T>;
    template <class T>
    using vector4D = Eigen::Vector4<T>;

    template <class T>
    using matrix2 = Eigen::Matrix2<T>;
    template <class T>
    using matrix3 = Eigen::Matrix3<T>;
    template <class T>
    using matrix4 = Eigen::Matrix4<T>;

    template <class T, int r = Eigen::Dynamic, int l = Eigen::Dynamic>
    using matrix = Eigen::Matrix<T, r, l>;

    template <class T, int l = Eigen::Dynamic>
    using matrixL = Eigen::Matrix<T, Eigen::Dynamic, l>;

    template <class T, int r = Eigen::Dynamic>
    using vector = Eigen::Vector<T, r>;

    template <class T>
    using ptr_s = std::shared_ptr<T>;
#if USE_CLN_DATA_STRUCTURE == 1
    using short_real_number = cln::cl_R;
    using real_number = cln::cl_R;
    using long_real_number = cln::cl_R;
    using byte_integer_number = cln::cl_I;
    using short_integer_number = cln::cl_I;
    using integer_number = cln::cl_I;
    using long_integer_number = cln::cl_I;

    using short_real_number_cref = const cln::cl_R &;
    using real_number_cref = const cln::cl_R &;
    using long_real_number_cref = const cln::cl_R &;
    using byte_integer_number_cref = const cln::cl_I &;
    using short_integer_number_cref = const cln::cl_I &;
    using integer_number_cref = const cln::cl_I &;
    using long_integer_number_cref = const cln::cl_I &;
#else
    using short_real_number = float;
    using real_number = double;
    using long_real_number = long double;

    using byte_integer_number = int8_t;
    using short_integer_number = int16_t;
    using integer_number = int32_t;
    using long_integer_number = int64_t;

    using short_real_number_cref = float;
    using real_number_cref = double;
    using long_real_number_cref = long double;
    using byte_integer_number_cref = int8_t;
    using short_integer_number_cref = int16_t;
    using integer_number_cref = int32_t;
    using long_integer_number_cref = int64_t;
#endif
    using str = std::string;
    using strv = std::string_view;
    using char_type = char;
    using str_stream = std::stringstream;
    using f_stream = std::fstream;
    using of_stream = std::ofstream;
    using if_stream = std::ifstream;

    class particle_basic_options {
     public:
        str particle_name {"minecraft:end_rod"};
        vector3D<real_number> coord {0, 0, 0};
        vector3D<real_number> initial_velocity {0, 0, 0};
        real_number velt_counting_period {1.0};
        integer_number age {0};
        bool relative_coord {false};

     public:
        particle_basic_options() = default;
        particle_basic_options(str particle_name_,
                               vector3D<real_number> coord_,
                               vector3D<real_number> initial_velocity_,
                               real_number_cref velt_counting_period_,
                               integer_number_cref age_,
                               bool relative_coord_);

        particle_basic_options(const particle_basic_options &) = default;
        particle_basic_options &operator=(const particle_basic_options &) = default;
        virtual ~particle_basic_options() = default;

        enum particle_option_type {
            EMPTY = 0,
            NORMAL = 1,
            CONDITIONAL = 2,
            PARAMETER = 4,
            TICKPARAMETER = PARAMETER + 8,
            RGBAPARAMETER = PARAMETER + 16,
            RGBATICKPARAMETER = TICKPARAMETER + 16,
            IMAGE = 32,
            IMAGEMATRIX = IMAGE + 64
        };

        [[nodiscard]] virtual particle_option_type get_option_type() const { return EMPTY; }
    };

    // /particleex normal
    class particle_normal_options : public particle_basic_options {
     public:
        vector4D<real_number> rgba {1.0, 1.0, 1.0, 1.0};
        vector3D<real_number> range {1.0, 1.0, 1.0};
        integer_number count {100};

     public:
        particle_normal_options() : particle_basic_options() {}
        particle_normal_options(vector4D<real_number> rgba_,
                                vector3D<real_number> range_, integer_number_cref count_,
                                const particle_basic_options &parent);

        particle_normal_options(const particle_normal_options &) = default;
        particle_normal_options &operator=(const particle_normal_options &) = default;
        ~particle_normal_options() override = default;

        [[nodiscard]] particle_option_type get_option_type() const override { return NORMAL; }

        [[nodiscard]] str format(const str &velocity_expr, integer_number_cref length = 0, integer_number_cref prec = 6) const;
    };

    // /particleex conditional
    class particle_conditional_options : public particle_basic_options {
     public:
        vector4D<real_number> rgba {1.0, 1.0, 1.0, 1.0};
        vector3D<real_number> range {1.0, 1.0, 1.0};
        real_number counting_period {0.1};

     public:
        particle_conditional_options() : particle_basic_options() {}
        particle_conditional_options(vector4D<real_number> rgba_,
                                     vector3D<real_number> range_, real_number_cref counting_period_,
                                     const particle_basic_options &parent);

        particle_conditional_options(const particle_conditional_options &) = default;
        particle_conditional_options &operator=(const particle_conditional_options &) = default;
        ~particle_conditional_options() override = default;

        [[nodiscard]] particle_option_type get_option_type() const override { return CONDITIONAL; }

        [[nodiscard]] str format(const str &conditional_expr, const str &velocity_expr, integer_number_cref length = 0, integer_number_cref prec = 6) const;
    };

    class parameter_particle_options : public particle_basic_options {
     private:
        vector2D<real_number> t_begin_end {0.0, 10.0};
        integer_number particle_num {100};
        real_number para_counting_period {0.1};
        bool polar {false};
        bool matching {false};

     public:
        parameter_particle_options() : particle_basic_options() {}
        parameter_particle_options(vector2D<real_number> t_begin_end_, integer_number_cref particle_num_, real_number_cref para_count_period_, bool polar_,
                                   const particle_basic_options &parent);

        parameter_particle_options(const parameter_particle_options &) = default;
        parameter_particle_options &operator=(const parameter_particle_options &) = default;
        ~parameter_particle_options() override = default;

        [[nodiscard]] const vector2D<real_number> &get_t_begin_end() const { return t_begin_end; }
        [[nodiscard]] integer_number_cref get_particle_num() const { return particle_num; }
        [[nodiscard]] real_number_cref get_para_counting_period() const { return para_counting_period; }

        [[nodiscard]] bool get_polar() const { return polar; }
        void set_polar(bool polar_) { polar = polar_; }

        void set_t_begin_end(const vector2D<real_number> &tbe) {
            t_begin_end = tbe;
            matching = false;
        }
        void set_t_begin_end(vector2D<real_number> &&tbe) {
            t_begin_end = std::move(tbe);
            matching = false;
        }
        void set_particle_num(integer_number_cref pn) {
            particle_num = pn;
            matching = false;
        }
        void set_para_counting_period(real_number_cref pcp) {
            para_counting_period = pcp;
            matching = false;
        }

        [[nodiscard]] bool get_matching() const { return matching; }

        vector2D<real_number> confirm_t_begin_end();
        real_number confirm_para_counting_period();
        integer_number confirm_particle_num();
    };

    // /particleex parameter /particleex polarparameter
    class particle_parameter_options : public parameter_particle_options {
     public:
        vector4D<real_number> rgba {1.0, 1.0, 1.0, 1.0};

     public:
        particle_parameter_options() : parameter_particle_options() {}
        particle_parameter_options(vector4D<real_number> rgba_,
                                   const parameter_particle_options &parent);

        particle_parameter_options(const particle_parameter_options &) = default;
        particle_parameter_options &operator=(const particle_parameter_options &) = default;
        ~particle_parameter_options() override = default;

        [[nodiscard]] particle_option_type get_option_type() const override { return PARAMETER; }

        [[nodiscard]] str format(const str &parameter_expr, const str &velocity_expr, integer_number_cref length = 0, integer_number_cref prec = 6) const;
    };
    // /particleex rgbaparameter /particleex rgbapolarparameter
    class particle_rgba_parameter_options : public parameter_particle_options {
     public:
        particle_rgba_parameter_options() = default;
        explicit particle_rgba_parameter_options(const parameter_particle_options &parent);
        particle_rgba_parameter_options(const particle_rgba_parameter_options &) = default;
        particle_rgba_parameter_options &operator=(const particle_rgba_parameter_options &) = default;
        ~particle_rgba_parameter_options() = default;

        [[nodiscard]] particle_option_type get_option_type() const override { return RGBAPARAMETER; }
        [[nodiscard]] str format(const str &rgba_parameter_expr, const str &velocity_expr, integer_number_cref length = 0, integer_number_cref prec = 6) const;
    };

    class tick_parameter_particle_options : public parameter_particle_options {
     public:
        integer_number gent_counting_per_tick {10};

     public:
        tick_parameter_particle_options() : parameter_particle_options() {}
        tick_parameter_particle_options(integer_number_cref gent_counting_per_tick_,
                                        const parameter_particle_options &parent);

        tick_parameter_particle_options(const tick_parameter_particle_options &) = default;
        tick_parameter_particle_options &operator=(const tick_parameter_particle_options &) = default;
        ~tick_parameter_particle_options() override = default;

        integer_number calculate_tick_number();
        [[nodiscard]] integer_number calculate_tick_number() const;
    };

    // /particleex tickparameter /particleex tickpolarparameter
    class particle_tick_parameter_options : public tick_parameter_particle_options {
     public:
        vector4D<real_number> rgba = {1.0, 1.0, 1.0, 1.0};

        particle_tick_parameter_options() : tick_parameter_particle_options() {}
        explicit particle_tick_parameter_options(vector4D<real_number> rgba_,
                                                 const tick_parameter_particle_options &parent);

        particle_tick_parameter_options(const particle_tick_parameter_options &) = default;
        particle_tick_parameter_options &operator=(const particle_tick_parameter_options &) = default;
        ~particle_tick_parameter_options() override = default;

        [[nodiscard]] particle_option_type get_option_type() const override { return TICKPARAMETER; }
        [[nodiscard]] str format(const str &tick_parameter_expr, const str &velocity_expr, integer_number_cref length = 0, integer_number_cref prec = 6) const;
    };



    // /particleex rgbatickparameter /particleex rgbatickpolarparameter
    class particle_rgba_tick_parameter_options : public tick_parameter_particle_options {
     public:
        particle_rgba_tick_parameter_options() : tick_parameter_particle_options() {}
        explicit particle_rgba_tick_parameter_options(const tick_parameter_particle_options &parent);

        particle_rgba_tick_parameter_options(const particle_rgba_tick_parameter_options &) = default;
        particle_rgba_tick_parameter_options &operator=(const particle_rgba_tick_parameter_options &) = default;
        ~particle_rgba_tick_parameter_options() override = default;

        [[nodiscard]] particle_option_type get_option_type() const override { return RGBATICKPARAMETER; }
        [[nodiscard]] str format(const str &rgba_tick_parameter_expr, const str &velocity_expr, integer_number_cref length = 0, integer_number_cref prec = 6) const;
    };

    class image_particle_options : public particle_basic_options {
     public:
        str path {"./default.png"};
        real_number ratio {1.0};
        integer_number count_per_block {10};


     public:
        image_particle_options() : particle_basic_options() {}
        image_particle_options(str path_,
                               real_number_cref ratio_,
                               integer_number_cref count_per_block_,
                               const particle_basic_options &parent);

        image_particle_options(const image_particle_options &) = default;
        image_particle_options &operator=(const image_particle_options &) = default;
        ~image_particle_options() override = default;
    };

    // /particleex image
    class particle_image_options : public image_particle_options {
     public:
        enum overturn_mode {
            zero,
            vertical,
            horizontally
        };

     public:
        integer_number x_turn;
        integer_number y_turn;
        integer_number z_turn;
        overturn_mode overturn_mode;

     public:
        particle_image_options() : image_particle_options(),
                                   x_turn(0),
                                   y_turn(0),
                                   z_turn(0),
                                   overturn_mode(zero) {}
        particle_image_options(
            integer_number_cref x_turn_,
            integer_number_cref y_turn_,
            integer_number_cref z_turn_,
            enum particle_image_options::overturn_mode overturn_mode_,
            const image_particle_options &parent);


        particle_image_options(const particle_image_options &) = default;
        particle_image_options &operator=(const particle_image_options &) = default;
        ~particle_image_options() override = default;

        [[nodiscard]] particle_option_type get_option_type() const override { return IMAGE; }

        [[nodiscard]] str format(const str &velocity_expr, integer_number_cref length = 0, integer_number_cref prec = 6) const;
    };

    // /particleex imagematrix
    class particle_image_matrix_options : public image_particle_options {
     public:
        Eigen::Matrix3<real_number> turn_matrix;

     public:
        particle_image_matrix_options() : image_particle_options(),
                                          turn_matrix {{1.0, 0.0, 0.0},
                                                       {0.0, 1.0, 0.0},
                                                       {0.0, 0.0, 1.0}} {}
        particle_image_matrix_options(Eigen::Matrix3<real_number> turn_matrix_,
                                      const image_particle_options &parent);


        particle_image_matrix_options(const particle_image_matrix_options &) = default;
        particle_image_matrix_options &operator=(const particle_image_matrix_options &) = default;
        ~particle_image_matrix_options() override = default;

        [[nodiscard]] particle_option_type get_option_type() const override { return IMAGEMATRIX; }
        [[nodiscard]] str format(const str &velocity_expr, integer_number_cref length = 0, integer_number_cref prec = 6) const;
    };

    integer_number parameter_real_particle_num(double t_begin, double t_end,
                                               double delta);


    str float_scientific_to_fix(const str &float_str, char_type exp_char = 'e', char_type dot_char = '.');


#if USE_CLN_DATA_STRUCTURE == 1
    char_type infer_exponent_char(const cln::cl_print_flags &flags, real_number_cref num);
#else
    str float_scientific_to_fix(const str &float_str, integer_number_cref length, integer_number_cref prec);
#endif

    template <class... Args>
    str dynamic_format(strv fmt, Args &&...args) {
        return std::vformat(fmt, std::make_format_args(std::forward<Args &&>(args)...));
    }

    struct float_spec {
        real_number num;
        integer_number length;
        integer_number spec;
    };


} // namespace mc_particle


template <>
class std::formatter<mc_particle::float_spec, mc_particle::char_type> {
 public:
    template <class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext &ctx) {
        auto it = ctx.begin();
        while (*it != '}')
            ++it;
        return it;
    }

    template <class FmtContext>
    FmtContext::iterator format(mc_particle::float_spec s, FmtContext &ctx) const {
        std::string fmt = std::format("({:{}.{}f})", s.num, s.length, s.spec);

        return std::ranges::copy(std::move(fmt), ctx.out()).out;
    }
};