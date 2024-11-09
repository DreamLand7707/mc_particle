
#include "basic_particle.hpp"
#include <cln/types.h>
#include <cln/object.h>
#include <cln/exception.h>

namespace mc_particle
{
    static std::pair<integer_number_cref, integer_number_cref> length_prec = {0, 0};

    static str fmt_real(real_number_cref r) {
#if USE_CLN_DATA_STRUCTURE == 1
        str_stream st;
        st << r;
        return float_scientific_to_fix(st.str(), infer_exponent_char(cln::default_print_flags, r));
#else
        return std::format("{:{}.{}f}", r, length_prec.first, length_prec.second);
#endif
    }
} // namespace mc_particle

namespace mc_particle
{
    particle_basic_options::particle_basic_options(str name,
                                                   vector3D<real_number> coord_,
                                                   vector3D<real_number> initial_velocity_,
                                                   real_number_cref vcp,
                                                   integer_number_cref age_,
                                                   bool relative_)
        : particle_name(std::move(name)),
          coord(std::move(coord_)),
          initial_velocity(std::move(initial_velocity_)),
          velt_counting_period(vcp),
          age(age_),
          relative_coord(relative_) {}
} // namespace mc_particle
namespace mc_particle
{
    particle_normal_options::particle_normal_options(vector4D<real_number> rgba_,
                                                     vector3D<real_number> range_, integer_number_cref count_,
                                                     const particle_basic_options &parent)
        : particle_basic_options(parent), rgba(std::move(rgba_)), range(std::move(range_)), count(count_) {}


    [[nodiscard]] str particle_normal_options::format(const str &velocity_expr, integer_number_cref length, integer_number_cref prec) const {
        str_stream ssin;
        const char_type *coord_str = relative_coord ? "~" : "";
        length_prec = {length, prec};
        ssin << "particleex normal "
             << particle_name << ' '
             << coord_str << fmt_real(coord(0)) << ' ' << coord_str << fmt_real(coord(1)) << ' ' << coord_str << fmt_real(coord(2)) << ' '
             << fmt_real(rgba(0)) << ' ' << fmt_real(rgba(1)) << ' ' << fmt_real(rgba(2)) << ' ' << fmt_real(rgba(3)) << ' '
             << fmt_real(initial_velocity(0)) << ' ' << fmt_real(initial_velocity(1)) << ' ' << fmt_real(initial_velocity(2)) << ' '
             << fmt_real(range(0)) << ' ' << fmt_real(range(1)) << ' ' << fmt_real(range(2)) << ' '
             << count << ' '
             << age << ' '
             << '\"' << velocity_expr << '\"' << ' '
             << fmt_real(velt_counting_period);

        return ssin.str();
    }
} // namespace mc_particle

namespace mc_particle
{
    particle_conditional_options::particle_conditional_options(vector4D<real_number> rgba_,
                                                               vector3D<real_number> range_, real_number_cref cp,
                                                               const particle_basic_options &parent)
        : particle_basic_options(parent), rgba(std::move(rgba_)), range(std::move(range_)), counting_period(cp) {}




    [[nodiscard]] str particle_conditional_options::format(const str &conditional_expr, const str &velocity_expr, integer_number_cref length, integer_number_cref prec) const {
        str_stream ssin;
        const char_type *coord_str = relative_coord ? "~" : "";
        length_prec = {length, prec};
        ssin << "particleex conditional "
             << particle_name << ' '
             << coord_str << fmt_real(coord(0)) << ' ' << coord_str << fmt_real(coord(1)) << ' ' << coord_str << fmt_real(coord(2)) << ' '
             << fmt_real(rgba(0)) << ' ' << fmt_real(rgba(1)) << ' ' << fmt_real(rgba(2)) << ' ' << fmt_real(rgba(3)) << ' '
             << fmt_real(initial_velocity(0)) << ' ' << fmt_real(initial_velocity(1)) << ' ' << fmt_real(initial_velocity(2)) << ' '
             << fmt_real(range(0)) << ' ' << fmt_real(range(1)) << ' ' << fmt_real(range(2)) << ' '
             << '\"' << conditional_expr << '\"' << ' '
             << fmt_real(counting_period) << ' '
             << age << ' '
             << '\"' << velocity_expr << '\"' << ' '
             << fmt_real(velt_counting_period);

        return ssin.str();
    }
} // namespace mc_particle

namespace mc_particle
{
    parameter_particle_options::parameter_particle_options(vector2D<real_number> tbe,
                                                           integer_number_cref pn,
                                                           real_number_cref vcp,
                                                           bool polar_,
                                                           const particle_basic_options &parent)
        : particle_basic_options(parent), t_begin_end(std::move(tbe)), particle_num(pn), para_counting_period(vcp), polar(polar_) {}

    vector2D<real_number> parameter_particle_options::confirm_t_begin_end() {
        if (!matching) {
            t_begin_end(1) = t_begin_end(0) +
                             (particle_num - 1) * para_counting_period;
            matching = true;
        }
        return t_begin_end;
    }
    real_number parameter_particle_options::confirm_para_counting_period() {
        if (!matching) {
            para_counting_period = (t_begin_end(1) - t_begin_end(0)) / (particle_num - 1);
            matching = true;
        }
        return para_counting_period;
    }
    integer_number parameter_particle_options::confirm_particle_num() {
        if (!matching) {
#if USE_CLN_DATA_STRUCTURE == 1
            particle_num = parameter_real_particle_num(double_approx(t_begin_end(0)),
                                                       double_approx(t_begin_end(1)),
                                                       double_approx(para_counting_period));
#else
            particle_num = parameter_real_particle_num(t_begin_end(0),
                                                       t_begin_end(1),
                                                       para_counting_period);
#endif
            t_begin_end(1) = t_begin_end(0) +
                             (particle_num - 1) * para_counting_period;
            matching = true;
        }
        return particle_num;
    }
} // namespace mc_particle

namespace mc_particle
{
    particle_parameter_options::particle_parameter_options(vector4D<real_number> rgba_,
                                                           const parameter_particle_options &parent)
        : parameter_particle_options(parent), rgba(std::move(rgba_)) {}



    [[nodiscard]] str particle_parameter_options::format(const str &parameter_expr, const str &velocity_expr, integer_number_cref length, integer_number_cref prec) const {
        str_stream ssin;
        const char_type *coord_str = relative_coord ? "~" : "";
        length_prec = {length, prec};
        ssin << "particleex " << (get_polar() ? "polarparameter " : "parameter ")
             << particle_name << ' '
             << coord_str << fmt_real(coord(0)) << ' ' << coord_str << fmt_real(coord(1)) << ' ' << coord_str << fmt_real(coord(2)) << ' '
             << fmt_real(rgba(0)) << ' ' << fmt_real(rgba(1)) << ' ' << fmt_real(rgba(2)) << ' ' << fmt_real(rgba(3)) << ' '
             << fmt_real(initial_velocity(0)) << ' ' << fmt_real(initial_velocity(1)) << ' ' << fmt_real(initial_velocity(2)) << ' '
             << fmt_real(get_t_begin_end()(0)) << ' ' << fmt_real(get_t_begin_end()(1) + get_para_counting_period() / 2.0) << ' '
             << '\"' << parameter_expr << '\"' << ' '
             << fmt_real(get_para_counting_period()) << ' '
             << age << ' '
             << '\"' << velocity_expr << '\"' << ' '
             << fmt_real(velt_counting_period);

        return ssin.str();
    }
} // namespace mc_particle

namespace mc_particle
{
    particle_rgba_parameter_options::particle_rgba_parameter_options(const parameter_particle_options &parent)
        : parameter_particle_options(parent) {}



    [[nodiscard]] str particle_rgba_parameter_options::format(const str &rgba_parameter_expr, const str &velocity_expr, integer_number_cref length, integer_number_cref prec) const {
        str_stream ssin;
        const char_type *coord_str = relative_coord ? "~" : "";
        length_prec = {length, prec};
        ssin << "particleex " << (get_polar() ? "rgbapolarparameter " : "rgbaparameter ")
             << particle_name << ' '
             << coord_str << fmt_real(coord(0)) << ' ' << coord_str << fmt_real(coord(1)) << ' ' << coord_str << fmt_real(coord(2)) << ' '
             << fmt_real(initial_velocity(0)) << ' ' << fmt_real(initial_velocity(1)) << ' ' << fmt_real(initial_velocity(2)) << ' '
             << fmt_real(get_t_begin_end()(0)) << ' ' << fmt_real(get_t_begin_end()(1) + get_para_counting_period() / 2.0) << ' '
             << rgba_parameter_expr << ' '
             << fmt_real(get_para_counting_period()) << ' '
             << age << ' '
             << '\"' << velocity_expr << '\"' << ' '
             << fmt_real(velt_counting_period);

        return ssin.str();
    }
} // namespace mc_particle

namespace mc_particle
{
    tick_parameter_particle_options::tick_parameter_particle_options(integer_number_cref gcpt,
                                                                     const parameter_particle_options &parent)
        : parameter_particle_options(parent), gent_counting_per_tick(gcpt) {}


    integer_number tick_parameter_particle_options::calculate_tick_number() {
        if (!get_matching()) {
            confirm_particle_num();
        }
#if USE_CLN_DATA_STRUCTURE == 1
        return cln::max(cln::ceiling1(get_particle_num(), gent_counting_per_tick) - 1, 0);
#else
        const integer_number &n = get_particle_num();
        if (n % gent_counting_per_tick)
            return std::max((n / gent_counting_per_tick) - 1, (integer_number)0);
        else
            return std::max((n / gent_counting_per_tick), (integer_number)0);
#endif
    }
    [[nodiscard]] integer_number tick_parameter_particle_options::calculate_tick_number() const {
#if USE_CLN_DATA_STRUCTURE == 1
        return cln::max(cln::ceiling1(get_particle_num(), gent_counting_per_tick) - 1, 0);
#else
        const integer_number &n = get_particle_num();
        if (n % gent_counting_per_tick)
            return std::max((n / gent_counting_per_tick) - 1, (integer_number)0);
        else
            return std::max((n / gent_counting_per_tick), (integer_number)0);
#endif
    }
} // namespace mc_particle

namespace mc_particle
{
    particle_tick_parameter_options::particle_tick_parameter_options(vector4D<real_number> rgba_,
                                                                     const tick_parameter_particle_options &parent)
        : tick_parameter_particle_options(parent), rgba(std::move(rgba_)) {}

    [[nodiscard]] str particle_tick_parameter_options::format(const str &tick_parameter_expr,
                                                              const str &velocity_expr,
                                                              integer_number_cref length, integer_number_cref prec) const {
        str_stream ssin;
        const char_type *coord_str = relative_coord ? "~" : "";
        length_prec = {length, prec};
        ssin << "particleex " << (get_polar() ? "tickpolarparameter " : "tickparameter ")
             << particle_name << ' '
             << coord_str << fmt_real(coord(0)) << ' ' << coord_str << fmt_real(coord(1)) << ' ' << coord_str << fmt_real(coord(2)) << ' '
             << fmt_real(rgba(0)) << ' ' << fmt_real(rgba(1)) << ' ' << fmt_real(rgba(2)) << ' ' << fmt_real(rgba(3)) << ' '
             << fmt_real(initial_velocity(0)) << ' ' << fmt_real(initial_velocity(1)) << ' ' << fmt_real(initial_velocity(2)) << ' '
             << fmt_real(get_t_begin_end()(0)) << ' ' << fmt_real(get_t_begin_end()(1) + get_para_counting_period() / 2.0) << ' '
             << '\"' << tick_parameter_expr << '\"' << ' '
             << fmt_real(get_para_counting_period()) << ' '
             << gent_counting_per_tick << ' '
             << age << ' '
             << '\"' << velocity_expr << '\"' << ' '
             << fmt_real(velt_counting_period);

        return ssin.str();
    }
} // namespace mc_particle


namespace mc_particle
{
    particle_rgba_tick_parameter_options::particle_rgba_tick_parameter_options(const tick_parameter_particle_options &parent)
        : tick_parameter_particle_options(parent) {}


    [[nodiscard]] str particle_rgba_tick_parameter_options::format(const str &rgba_tick_parameter_expr, const str &velocity_expr, integer_number_cref length, integer_number_cref prec) const {
        str_stream ssin;
        const char_type *coord_str = relative_coord ? "~" : "";
        length_prec = {length, prec};
        ssin << "particleex " << (get_polar() ? "rgbatickpolarparameter " : "rgbatickparameter ")
             << particle_name << ' '
             << coord_str << fmt_real(coord(0)) << ' ' << coord_str << fmt_real(coord(1)) << ' ' << coord_str << fmt_real(coord(2)) << ' '
             << fmt_real(initial_velocity(0)) << ' ' << fmt_real(initial_velocity(1)) << ' ' << fmt_real(initial_velocity(2)) << ' '
             << fmt_real(get_t_begin_end()(0)) << ' ' << fmt_real(get_t_begin_end()(1) + get_para_counting_period() / 2.0) << ' '
             << '\"' << rgba_tick_parameter_expr << '\"' << ' '
             << fmt_real(get_para_counting_period()) << ' '
             << gent_counting_per_tick << ' '
             << age << ' '
             << '\"' << velocity_expr << '\"' << ' '
             << fmt_real(velt_counting_period);

        return ssin.str();
    }
} // namespace mc_particle


namespace mc_particle
{
    image_particle_options::image_particle_options(str path_,
                                                   real_number_cref ratio_,
                                                   integer_number_cref cpb_,
                                                   const particle_basic_options &parent)
        : particle_basic_options(parent),
          path(std::move(path_)),
          ratio(ratio_),
          count_per_block(cpb_) {}
} // namespace mc_particle

namespace mc_particle
{
    particle_image_options::particle_image_options(
        integer_number_cref x_turn_,
        integer_number_cref y_turn_,
        integer_number_cref z_turn_,
        enum particle_image_options::overturn_mode om,
        const image_particle_options &parent)
        : image_particle_options(parent), x_turn(x_turn_), y_turn(y_turn_), z_turn(z_turn_), overturn_mode(om) {}



    [[nodiscard]] str particle_image_options::format(const str &velocity_expr, integer_number_cref length, integer_number_cref prec) const {
        str_stream ssin;
        const char_type *coord_str = relative_coord ? "~" : "";
        length_prec = {length, prec};
        ssin << "particleex image " << particle_name << ' ' << coord_str
             << fmt_real(coord(0)) << ' ' << coord_str << fmt_real(coord(1))
             << ' ' << coord_str << fmt_real(coord(2)) << ' ' << '\"' << path
             << '\"' << ' '  // path 两边加上双引号
             << fmt_real(ratio) << ' ' << x_turn << ' ' << y_turn << ' '
             << z_turn << ' '
             << (overturn_mode == particle_image_options::zero
                     ? "not "
                     : (overturn_mode == particle_image_options::vertical
                            ? "vertical "
                            : "horizontally "))
             << count_per_block << ' ' << fmt_real(initial_velocity(0)) << ' '
             << fmt_real(initial_velocity(1)) << ' '
             << fmt_real(initial_velocity(2)) << ' ' << age << ' ' << '\"'
             << velocity_expr << '\"' << ' ' << fmt_real(velt_counting_period);

        return ssin.str();
    }
} // namespace mc_particle


namespace mc_particle
{

    particle_image_matrix_options::particle_image_matrix_options(Eigen::Matrix3<real_number> turn_matrix_,
                                                                 const image_particle_options &parent)
        : image_particle_options(parent), turn_matrix(std::move(turn_matrix_)) {}


    [[nodiscard]] str particle_image_matrix_options::format(const str &velocity_expr, integer_number_cref length, integer_number_cref prec) const {
        str_stream ssin;
        const char_type *coord_str = relative_coord ? "~" : "";
        length_prec = {length, prec};
        ssin << "particleex imagematrix "
             << particle_name << ' '
             << coord_str << fmt_real(coord(0)) << ' ' << coord_str << fmt_real(coord(1)) << ' ' << coord_str << fmt_real(coord(2)) << ' '
             << path << ' '
             << fmt_real(ratio) << ' '
             << '(';
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                ssin << fmt_real(turn_matrix(i, j));
                if (i != 2 && j != 2)
                    ssin << ',';
            }
            if (i != 2)
                ssin << ',';
        }
        ssin << ')' << ' '
             << count_per_block << ' '
             << fmt_real(initial_velocity(0)) << ' ' << fmt_real(initial_velocity(1)) << ' ' << fmt_real(initial_velocity(2)) << ' '
             << age << ' '
             << '\"' << velocity_expr << '\"' << ' '
             << fmt_real(velt_counting_period);

        return ssin.str();
    }
} // namespace mc_particle

namespace mc_particle
{
    integer_number parameter_real_particle_num(double t_begin, double t_end, double delta) {
        integer_number count;
        for (count = 0; t_begin <= t_end; count++) {
            t_begin += delta;
        }
        return count;
    }


#if USE_CLN_DATA_STRUCTURE == 1
    str float_scientific_to_fix(const str &float_str, char_type exp_char, char_type dot_char) {
        str ts = float_str;
        if (ts[0] == '-')
            ts[0] = '0';
        str::size_type n = ts.size();
        str::size_type e_pos, dot_pos;
        e_pos = ts.find(exp_char);
        dot_pos = ts.find(dot_char);

        if (e_pos == str::npos)
            e_pos = n;

        bool zero_all = true;
        for (int64_t i = 0; i < e_pos; i++) {
            if (ts[i] >= '1' && ts[i] <= '9') {
                zero_all = false;
                break;
            }
        }
        if (zero_all)
            return "0.0";

        int64_t str_begin = 0, str_end = (int64_t)e_pos;

        int64_t expoe = 0;
        int64_t temp1 = 0, temp2 = 0, temp3 = 0;
        if (str_end != n || dot_pos != 0 || dot_pos != str::npos) {
            if (e_pos == n)
                expoe = 0;
            else
                expoe = std::stoi(ts.substr(e_pos + 1));
            temp1 = (dot_pos == str::npos) ? (str_end + expoe) : ((int64_t)dot_pos + expoe);
            temp1 -= str_begin;
            temp3 = str_end - str_begin;
            if (dot_pos == str::npos) { // no dot_char
                if (temp1 >= temp3)
                    temp2 = temp1 + 2;
                else if (temp1 < temp3 && temp1 > 0)
                    temp2 = temp3 + 1;
                else
                    temp2 = (int64_t)temp3 + std::abs(temp1) + 2;
            }
            else { // have dot_char
                if (temp1 == (int64_t)(temp3 - 1) || temp1 == 0)
                    temp2 = temp3 + 1;
                else if (temp1 > (int64_t)(temp3 - 1))
                    temp2 = temp1 + 2;
                else if (temp1 < 0)
                    temp2 = (int64_t)temp3 + std::abs(temp1) + 1;
                else
                    temp2 = temp3;
            }
            str ret(temp2, '0');
            temp3 = 0;
            bool have_dot = false;
            for (int64_t i = 0; i < temp2;) {
                if (i == 0 && temp1 <= 0) {
                    ret[1] = dot_char;
                    have_dot = true;
                    i += 2;
                    continue;
                }

                if (temp1 < 0) {
                    i += std::abs(temp1);
                    temp1 = 0;
                    continue;
                }

                if (str_begin < str_end) {
                    if (i == temp1) {
                        ret[i] = dot_char;
                        have_dot = true;
                        i++;
                        continue;
                    }
                    else if (ts[str_begin] == dot_char) {
                        str_begin++;
                        continue;
                    }
                    else {
                        ret[i] = ts[str_begin];
                        i++;
                        str_begin++;
                        continue;
                    }
                }

                if (!have_dot)
                    ret[temp2 - 2] = dot_char;
                break;
            }

            str_begin = -1, str_end = temp2 + 1;
            bool zero_up = false, get_dot = false;
            for (int64_t i = 0; i < temp2; i++) {
                if (ret[i] == '0') {
                    if (!get_dot)
                        zero_up = true;
                }
                else {
                    if (str_begin == -1)
                        str_begin = (zero_up && ret[i] == dot_char) ? i - 1 : i;
                    if (ret[i] == dot_char)
                        get_dot = true;
                    str_end = (ret[i] == dot_char ? i + 1 : i) + 1;
                }
            }
            if (str_begin < 0)
                str_begin = 0;
            if (str_end == temp2 + 1)
                str_end = temp2;

            return (float_str[0] == '-' ? "-" : "") + ret.substr(str_begin, str_end - str_begin);
        }
        else {
            return float_str;
        }
    }
#else

    str float_scientific_to_fix(const str &float_str, integer_number_cref length, integer_number_cref prec) {
        return std::format("{:{}.{}f}", std::stod(float_str), length, prec);
    }

#endif

#if USE_CLN_DATA_STRUCTURE == 1
#define ceiling(a_from_ceiling, b_from_ceiling) \
    (((a_from_ceiling) + (b_from_ceiling) - 1) / (b_from_ceiling))

#define floatformatcase(value, SF_statement, FF_statement, DF_statement, LF_statement) \
    {                                                                                  \
        if ((value) <= cln::float_format_sfloat) {                                     \
            SF_statement                                                               \
        }                                                                              \
        else if ((value) <= cln::float_format_ffloat) {                                \
            FF_statement                                                               \
        }                                                                              \
        else if ((value) <= cln::float_format_dfloat) {                                \
            DF_statement                                                               \
        }                                                                              \
        else {                                                                         \
            uintC len = ceiling((uintC)(value), intDsize);                             \
            LF_statement                                                               \
        }                                                                              \
    }

#define NOTREACHED throw cln::notreached_exception(__FILE__, __LINE__);

#ifdef CL_WIDE_POINTERS
#define floattypecase(x, SF_statement, FF_statement, DF_statement, LF_statement) \
    if (!(x).pointer_p())                                                        \
        switch ((x).nonpointer_tag()) {                                          \
        case cl_SF_tag: {                                                        \
            SF_statement                                                         \
        } break;                                                                 \
        case cl_FF_tag: {                                                        \
            FF_statement                                                         \
        } break;                                                                 \
        default:                                                                 \
            NOTREACHED                                                           \
        }                                                                        \
    else {                                                                       \
        if ((x).pointer_type() == &cln::cl_class_dfloat) {                       \
            DF_statement                                                         \
        }                                                                        \
        else if ((x).pointer_type() == &cln::cl_class_lfloat) {                  \
            LF_statement                                                         \
        }                                                                        \
        else                                                                     \
            NOTREACHED                                                           \
    }
#else
#define floattypecase(x, SF_statement, FF_statement, DF_statement, LF_statement) \
    if (!(x).pointer_p())                                                        \
        switch ((x).nonpointer_tag()) {                                          \
        case cl_SF_tag: {                                                        \
            SF_statement                                                         \
        } break;                                                                 \
        default:                                                                 \
            NOTREACHED                                                           \
        }                                                                        \
    else {                                                                       \
        if ((x).pointer_type() == &cl_class_ffloat) {                            \
            FF_statement                                                         \
        }                                                                        \
        else if ((x).pointer_type() == &cl_class_dfloat) {                       \
            DF_statement                                                         \
        }                                                                        \
        else if ((x).pointer_type() == &cl_class_lfloat) {                       \
            LF_statement                                                         \
        }                                                                        \
        else                                                                     \
            NOTREACHED                                                           \
    }
#endif
    static bool rationalp(const cln::cl_R &x) {
        if (!x.pointer_p()) {
            if (x.nonpointer_tag() == cl_FN_tag)
                return true;
        }
        else {
            if ((unsigned int)(x.pointer_type()->flags) & cl_class_flags_subclass_rational)
                return true;
        }
        return false;
    }
    char_type infer_exponent_char(const cln::cl_print_flags &flags, real_number_cref num) {
        if (rationalp(num))
            return '\0';
        char_type exp_marker;
        bool flag = (cln::abs(num) >= cln::cl_F(0.001)) && (cln::abs(num) < cln::cl_F(10000000.0));
        floattypecase(num, exp_marker = 's';
                      , exp_marker = 'f';
                      , exp_marker = 'd';
                      , exp_marker = 'L';);

        struct cl_heap_lfloat : cln::cl_heap {
            uintC len;     // length of mantissa (in digits)
            int sign;      // sign (0 or -1)
            uintE expo;    // exponent
            uintD data[1]; // mantissa
        };

        if (!flags.float_readably) {
            floatformatcase(flags.default_float_format, if (exp_marker == 's') { exp_marker = 'E'; }, if (exp_marker == 'f') { exp_marker = 'E'; }, if (exp_marker == 'd') { exp_marker = 'E'; }, if ((exp_marker == 'L') && (len == (((cl_heap_lfloat *)(num.pointer))->len))) { exp_marker = 'E'; });
        }

        if (!(flag && (exp_marker == 'E'))) { // evtl. Exponent ganz weglassen
            return exp_marker;
        }
        else {
            return '\0';
        }
    }

#undef ceiling
#undef elif
#undef floatformatcase
#undef NOTREACHED
#undef floattypecase
#endif
} // namespace mc_particle