#pragma once
#include <utility>

#include "../basic_particle.hpp"


namespace mc_particle::svg
{
    class dim2_parameterized_line;
    class dim2_parameterized_circle;
    class dim2_parameterized_ellipse;
    class dim2_parameterized_arc;
    class dim2_parameterized_elliarc;
    class dim2_parameterized_quad_bezier;
    class dim2_parameterized_cube_bezier;

    // 可一维参数化的二维对象
    class dim2_parameterized_dim1 {
     public:
        enum class para_type : int {
            error = -1,
            line = 0,
            circle = 1,
            ellipse = 2,
            arc = 3,
            elliarc = 4,
            quad_bezier = 5,
            cube_bezier = 6
        };

        [[nodiscard]] virtual vector2Dr get_param_range() const = 0;
        [[nodiscard]] virtual vector2Dr get_plot_ratio() const = 0;
        [[nodiscard]] virtual vector2Dr operator()(real_number_cref t) const { return get_line_value(t); }
        [[nodiscard]] virtual vector2Dr get_line_value(real_number_cref t) const = 0;
        [[nodiscard]] virtual para_type type() const = 0;

    private:
        template <class T, class Func>
        static T checkout_valid_function_obj_detail(const dim2_parameterized_dim1 &param, Func &&func);
        template <class T, class Func>
        static T checkout_valid_function_ptr_detail(const dim2_parameterized_dim1 &param, Func *func);

     public:
        template <class T, class Func, class... Funcs>
        static T checkout_valid_function(const dim2_parameterized_dim1 &param, Func && func, Funcs &&... funcs);
        template <class T, class Func>
        static T checkout_valid_function(const dim2_parameterized_dim1 &param, Func && func);
    };
    class dim2_parameterized_line : public dim2_parameterized_dim1 {
        vector2Dr from;
        vector2Dr to;
        vector2Dr ratio;

     public:
        dim2_parameterized_line() : from {0, 0}, to {1, 1}, ratio {1, 1} {};

        dim2_parameterized_line(vector2Dr from_, vector2Dr to_, vector2Dr ratio_ = {1, 1})
            : from(std::move(from_)), to(std::move(to_)), ratio(std::move(ratio_)) {};

        dim2_parameterized_line(const dim2_parameterized_line &rhs)
            : from(rhs.from), to(rhs.to), ratio(rhs.ratio) {};

        dim2_parameterized_line(dim2_parameterized_line &&rhs) noexcept
            : from(std::move(rhs.from)), to(std::move(rhs.to)), ratio(std::move(rhs.ratio)) {};

        dim2_parameterized_line &operator=(const dim2_parameterized_line &rhs) {
            if (this == &rhs) return *this;
            from = rhs.from;
            to = rhs.to;
            ratio = rhs.ratio;
            return *this;
        };

        dim2_parameterized_line &operator=(dim2_parameterized_line &&rhs) noexcept {
            from = std::move(rhs.from);
            to = std::move(rhs.to);
            ratio = std::move(rhs.ratio);
            return *this;
        };
        // from base
        [[nodiscard]] vector2Dr get_param_range() const override { return {0, ratio(0) / ratio(1)}; }
        [[nodiscard]] vector2Dr get_plot_ratio() const override { return ratio; }
        [[nodiscard]] vector2Dr get_line_value(real_number_cref t) const override;
        [[nodiscard]] para_type type() const override { return para_type::line; }

        // personalized
        [[nodiscard]] vector2Dr get_from() const { return from; }
        [[nodiscard]] vector2Dr get_to() const { return to; }
    };
    class dim2_parameterized_circle : public dim2_parameterized_dim1 {
        vector2Dr center;
        real_number radius;
        vector2Dr ratio;

     public:
        // from base
        [[nodiscard]] vector2Dr get_param_range() const override { return {0, ratio(0) / ratio(1)}; }
        [[nodiscard]] vector2Dr get_plot_ratio() const override { return ratio; }
        [[nodiscard]] vector2Dr get_line_value(real_number_cref t) const override;
        [[nodiscard]] para_type type() const override { return para_type::circle; }

        // personalized
        // TODO
    };
    class dim2_parameterized_ellipse : public dim2_parameterized_dim1 {
        vector2Dr center;
        vector2Dr half_axs_length;
        vector2Dr ratio;

     public:
        [[nodiscard]] vector2Dr get_param_range() const override { return {0, ratio(0) / ratio(1)}; }
        [[nodiscard]] vector2Dr get_plot_ratio() const override { return ratio; }
        [[nodiscard]] vector2Dr get_line_value(real_number_cref t) const override { return {0, 0}; }
        [[nodiscard]] para_type type() const override { return para_type::ellipse; }
    };
    class dim2_parameterized_arc : public dim2_parameterized_dim1 {
        vector2Dr center;
        real_number radius;
        vector2Dr angle_from_to;
        vector2Dr ratio;

     public:
        [[nodiscard]] vector2Dr get_param_range() const override { return {0, ratio(0) / ratio(1)}; }
        [[nodiscard]] vector2Dr get_plot_ratio() const override { return ratio; }
        [[nodiscard]] vector2Dr get_line_value(real_number_cref t) const override { return {0, 0}; }
        [[nodiscard]] para_type type() const override { return para_type::arc; }
    };
    class dim2_parameterized_elliarc : public dim2_parameterized_dim1 {
        vector2Dr center;
        vector2Dr half_axs_length;
        vector2Dr angle_from_to;
        vector2Dr ratio;

     public:
        [[nodiscard]] vector2Dr get_param_range() const override { return {0, ratio(0) / ratio(1)}; }
        [[nodiscard]] vector2Dr get_plot_ratio() const override { return ratio; }
        [[nodiscard]] vector2Dr get_line_value(real_number_cref t) const override { return {0, 0}; }
        [[nodiscard]] para_type type() const override { return para_type::elliarc; }
    };
    class dim2_parameterized_quad_bezier : public dim2_parameterized_dim1 {
        std::optional<vector2Dr> from;
        std::optional<vector2Dr> ctrl;
        vector2Dr to;
        vector2Dr ratio;

     public:
        [[nodiscard]] vector2Dr get_param_range() const override { return {0, ratio(0) / ratio(1)}; }
        [[nodiscard]] vector2Dr get_plot_ratio() const override { return ratio; }
        [[nodiscard]] vector2Dr get_line_value(real_number_cref t) const override { return {0, 0}; }
        [[nodiscard]] para_type type() const override { return para_type::quad_bezier; }
    };
    class dim2_parameterized_cube_bezier : public dim2_parameterized_dim1 {
        std::optional<vector2Dr> from;
        std::optional<vector2Dr> ctrl_f;
        vector2Dr ctrl_t;
        vector2Dr to;
        vector2Dr ratio;

     public:
        [[nodiscard]] vector2Dr get_param_range() const override { return {0, ratio(0) / ratio(1)}; }
        [[nodiscard]] vector2Dr get_plot_ratio() const override { return ratio; }
        [[nodiscard]] vector2Dr get_line_value(real_number_cref t) const override { return {0, 0}; }
        [[nodiscard]] para_type type() const override { return para_type::cube_bezier; }
    };


    // 绘图元素
    class line_shape_element {
     public:
        virtual ptr_s<dim2_parameterized_dim1> operator[](size_t index) = 0;
        virtual size_t size() = 0;
        virtual bool empty() = 0;
        virtual bool good() = 0;
        virtual int type() = 0;
    };
    class line_shape_rect : public line_shape_element {};
    class line_shape_circle : public line_shape_element {};
    class line_shape_ellipse : public line_shape_element {};
    class line_shape_line : public line_shape_element {};
    class line_shape_polyline : public line_shape_element {};
    class line_shape_polygon : public line_shape_element {};
    class line_shape_path : public line_shape_element {};

    namespace _details
    {
        using para_type = dim2_parameterized_dim1::para_type;
    #define DIM2_USING(X) using dim2_parameterized_dim1::para_type::X
        DIM2_USING(error      );
        DIM2_USING(line       );
        DIM2_USING(circle     );
        DIM2_USING(ellipse    );
        DIM2_USING(arc        );
        DIM2_USING(elliarc    );
        DIM2_USING(quad_bezier);
        DIM2_USING(cube_bezier);
    #undef DIM2_USING

        template <class T>
        struct param_type_number : std::integral_constant<para_type, error> { using _use_type = T; };
    #define PARAM_TYPE_NUMBER(X, Y)                                         \
        template<>                                                          \
        struct param_type_number<X> : std::integral_constant<para_type, Y>  \
        { using _use_type = X; }
        PARAM_TYPE_NUMBER(dim2_parameterized_line, line);
        PARAM_TYPE_NUMBER(dim2_parameterized_circle, circle);
        PARAM_TYPE_NUMBER(dim2_parameterized_ellipse, ellipse);
        PARAM_TYPE_NUMBER(dim2_parameterized_arc, arc);
        PARAM_TYPE_NUMBER(dim2_parameterized_elliarc, elliarc);
        PARAM_TYPE_NUMBER(dim2_parameterized_quad_bezier, quad_bezier);
        PARAM_TYPE_NUMBER(dim2_parameterized_cube_bezier, cube_bezier);
    #undef PARAM_TYPE_NUMBER

        template <class... ts>
        struct type_judge : param_type_number<void> {};
        template <class T>
        struct type_judge<T> : param_type_number<T> {};

        struct is_a_funcp;
        struct is_a_object;

        template <class except_return_type, class func, class judge1, bool>
        struct type_extract : type_judge<void> {};

        template <class T, class Ret, class... Args, bool _b>
        struct type_extract<T, Ret(Args...), std::enable_if_t<std::is_convertible_v<Ret, T>, is_a_funcp>, _b>
            : type_judge<std::remove_cvref_t<Args>...> {};

    #define TYPE_OBJ_EXTRACT(X)                                                                                                 \
        template <class T, class callable_obj, bool _b>                                                                         \
        struct type_extract <                                                                                                   \
            T,                                                                                                                  \
            callable_obj,                                                                                                       \
            std::enable_if_t <  std::is_invocable_v<callable_obj, const X &> &&                                                 \
                                    std::is_convertible_v<std::invoke_result_t<callable_obj, const X &>, T> &&                  \
                                    (   !                                                                                       \
                                        (                                                                                       \
                                            ( requires /* obj->const function->!const */                                        \
                                              (std::invoke_result_t<callable_obj, const X &> (callable_obj::*p) (const X &))    \
                                              { p = &callable_obj::operator(); }                                                \
                                            ) && _b                                                                             \
                                        )                                                                                       \
                                    ),                                                                                          \
                                is_a_object>,                                                                                   \
            _b>                                                                                                                 \
            : type_judge<X> {}

        TYPE_OBJ_EXTRACT(dim2_parameterized_line);
        TYPE_OBJ_EXTRACT(dim2_parameterized_circle);
        TYPE_OBJ_EXTRACT(dim2_parameterized_ellipse);
        TYPE_OBJ_EXTRACT(dim2_parameterized_arc);
        TYPE_OBJ_EXTRACT(dim2_parameterized_elliarc);
        TYPE_OBJ_EXTRACT(dim2_parameterized_quad_bezier);
        TYPE_OBJ_EXTRACT(dim2_parameterized_cube_bezier);
    #undef TYPE_OBJ_EXTRACT

        template <class Function, class T, class = void>
        struct ptr0_obj1 : std::false_type {
            template <class Func>
            static std::optional<T> _i_func(const mc_particle::svg::dim2_parameterized_dim1 &param, Func &&func) {
                using _type = type_extract<T, std::remove_cvref_t<Func>, is_a_funcp, std::is_const_v<Func>>;
                if (_type::value == param.type()) {
                    const auto &ref = dynamic_cast<const typename _type::_use_type &>(param);
                    return static_cast<T>(func(ref));
                }
                else {
                    return std::nullopt;
                }
            }
        };
        template <class Function, class F>
        struct ptr0_obj1<Function, void, F> : std::false_type {
            template <class Func>
            static std::optional<bool> _i_func(const mc_particle::svg::dim2_parameterized_dim1 &param, Func &&func) {
                using _type = type_extract<void, std::remove_cvref_t<Func>, is_a_funcp, std::is_const_v<Func>>;
                if (_type::value == param.type()) {
                    const auto &ref = dynamic_cast<const typename _type::_use_type &>(param);
                    func(ref);
                    return true;
                }
                else {
                    return std::nullopt;
                }
            }
        };
        template <class Function, class T>
        struct ptr0_obj1<Function, T, std::enable_if_t<std::is_class_v<std::remove_cvref_t<Function>>, void>> : std::true_type {
            template <class Func>
            static std::optional<T> _i_func(const mc_particle::svg::dim2_parameterized_dim1 &param, Func &&func) {
                using _type = type_extract<T, std::remove_cvref_t<Func>, is_a_object, std::is_const_v<Func>>;
                if (_type::value == param.type()) {
                    const auto &ref = dynamic_cast<const typename _type::_use_type &>(param);
                    return static_cast<T>(func(ref));
                }
                else {
                    return std::nullopt;
                }
            }
        };
        template <class Function>
        struct ptr0_obj1<Function, void, std::enable_if_t<std::is_class_v<std::remove_cvref_t<Function>>, void>> : std::true_type {
            template <class Func>
            static std::optional<bool> _i_func(const mc_particle::svg::dim2_parameterized_dim1 &param, Func &&func) {
                using _type = type_extract<void, std::remove_cvref_t<Func>, is_a_object, std::is_const_v<Func>>;
                if (_type::value == param.type()) {
                    const auto &ref = dynamic_cast<const typename _type::_use_type &>(param);
                    func(ref);
                    return true;
                }
                else {
                    return std::nullopt;
                }
            }
        };
    } // namespace _details
} // namespace mc_particle::svg


template <class T, class Func, class... Funcs>
T mc_particle::svg::dim2_parameterized_dim1::checkout_valid_function(const dim2_parameterized_dim1 &param, Func &&func, Funcs &&...funcs) {
    auto &&ret = _details::ptr0_obj1<std::remove_cvref_t<Func>, T>::_i_func(param, std::forward<Func &&>(func));
    if (ret.has_value()) {
        if constexpr (std::is_void_v<T>) {
            return;
        }
        else {
            return *ret;
        }
    }
    else {
        checkout_valid_function<T>(param, std::forward<Funcs &&>(funcs)...);
    }
}

template <class T, class Func>
T mc_particle::svg::dim2_parameterized_dim1::checkout_valid_function(const dim2_parameterized_dim1 &param, Func &&func) {
    auto &&ret = _details::ptr0_obj1<std::remove_cvref_t<Func>, T>::_i_func(param, std::forward<Func &&>(func));
    if (ret.has_value()) {
        if constexpr (std::is_void_v<T>) {
            return;
        }
        else {
            return *ret;
        }
    }
    else {
        throw std::runtime_error("Can't Invoke Any Function!");
    }
}
