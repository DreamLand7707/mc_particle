#pragma once // 防止头文件被重复包含

#include <utility>               // 引入标准库的 utility 头文件，提供工具函数
#include "../basic_particle.hpp" // 引入相对路径的 basic_particle.hpp 头文件

namespace mc_particle::svg // 定义命名空间 mc_particle::svg
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
        static T checkout_valid_function(const dim2_parameterized_dim1 &param, Func &&func, Funcs &&...funcs);
        template <class T, class Func>
        static T checkout_valid_function(const dim2_parameterized_dim1 &param, Func &&func);
    };
    class dim2_parameterized_line : public dim2_parameterized_dim1 {
        vector2Dr from;
        vector2Dr to;
        vector2Dr ratio;

     public:
        dim2_parameterized_line() : from {0, 0}, to {1, 1}, ratio {1, 1} {}; // 默认构造函数，初始化成员变量

        dim2_parameterized_line(vector2Dr from_, vector2Dr to_, vector2Dr ratio_ = {1, 1})
            : from(std::move(from_)), to(std::move(to_)), ratio(std::move(ratio_)) {}; // 带参数的构造函数

        dim2_parameterized_line(const dim2_parameterized_line &rhs)
            : from(rhs.from), to(rhs.to), ratio(rhs.ratio) {}; // 拷贝构造函数

        dim2_parameterized_line(dim2_parameterized_line &&rhs) noexcept
            : from(std::move(rhs.from)), to(std::move(rhs.to)), ratio(std::move(rhs.ratio)) {}; // 移动构造函数

        dim2_parameterized_line &operator=(const dim2_parameterized_line &rhs) {
            if (this == &rhs) return *this; // 防止自我赋值
            from = rhs.from;
            to = rhs.to;
            ratio = rhs.ratio;
            return *this;
        }; // 拷贝赋值运算符

        dim2_parameterized_line &operator=(dim2_parameterized_line &&rhs) noexcept {
            from = std::move(rhs.from);
            to = std::move(rhs.to);
            ratio = std::move(rhs.ratio);
            return *this;
        }; // 移动赋值运算符

        // 重写基类的纯虚函数
        [[nodiscard]] vector2Dr get_param_range() const override { return {0, ratio(0) / ratio(1)}; } // 获取参数范围
        [[nodiscard]] vector2Dr get_plot_ratio() const override { return ratio; }                     // 获取绘图比例
        [[nodiscard]] vector2Dr get_line_value(real_number_cref t) const override;                    // 根据参数 t 获取曲线上点
        [[nodiscard]] para_type type() const override { return para_type::line; }                     // 获取曲线类型

        // 个性化的成员函数
        [[nodiscard]] vector2Dr get_from() const { return from; } // 获取起点坐标
        [[nodiscard]] vector2Dr get_to() const { return to; }     // 获取终点坐标
    };
    class dim2_parameterized_circle : public dim2_parameterized_dim1 {
        vector2Dr center;
        real_number radius;
        vector2Dr ratio;

     public:
        // 默认构造函数
        dim2_parameterized_circle() : center {0, 0}, radius {1}, ratio {1, 1} {}

        // 带参数的构造函数
        dim2_parameterized_circle(vector2Dr center_, real_number radius_, vector2Dr ratio_ = {1, 1})
            : center(std::move(center_)), radius(radius_), ratio(std::move(ratio_)) {}

        // 拷贝构造函数
        dim2_parameterized_circle(const dim2_parameterized_circle &rhs)
            : center(rhs.center), radius(rhs.radius), ratio(rhs.ratio) {}

        // 移动构造函数
        dim2_parameterized_circle(dim2_parameterized_circle &&rhs) noexcept
            : center(std::move(rhs.center)), radius(rhs.radius), ratio(std::move(rhs.ratio)) {}

        // 拷贝赋值运算符
        dim2_parameterized_circle &operator=(const dim2_parameterized_circle &rhs) {
            if (this == &rhs) return *this;
            center = rhs.center;
            radius = rhs.radius;
            ratio = rhs.ratio;
            return *this;
        }

        // 移动赋值运算符
        dim2_parameterized_circle &operator=(dim2_parameterized_circle &&rhs) noexcept {
            if (this == &rhs) return *this;
            center = std::move(rhs.center);
            radius = rhs.radius;
            ratio = std::move(rhs.ratio);
            return *this;
        }

        // 析构函数
        ~dim2_parameterized_circle() = default;
        // 重写基类的纯虚函数
        [[nodiscard]] vector2Dr get_param_range() const override { return {0, ratio(0) / ratio(1)}; } // 获取参数范围
        [[nodiscard]] vector2Dr get_plot_ratio() const override { return ratio; }                     // 获取绘图比例
        [[nodiscard]] vector2Dr get_line_value(real_number_cref t) const override;                    // 根据参数 t 获取曲线上点
        [[nodiscard]] para_type type() const override { return para_type::circle; }                   // 获取曲线类型

        // 个性化的成员函数
        // TODO
    };
    class dim2_parameterized_ellipse : public dim2_parameterized_dim1 {
        vector2Dr center;
        vector2Dr half_axs_length;
        vector2Dr ratio;


     public:
        // 默认构造函数
        dim2_parameterized_ellipse() : center {0, 0}, half_axs_length {1, 1}, ratio {1, 1} {}

        // 带参数的构造函数
        dim2_parameterized_ellipse(vector2Dr center_, vector2Dr half_axs_length_, vector2Dr ratio_ = {1, 1})
            : center(std::move(center_)), half_axs_length(std::move(half_axs_length_)), ratio(std::move(ratio_)) {}

        // 拷贝构造函数
        dim2_parameterized_ellipse(const dim2_parameterized_ellipse &rhs)
            : center(rhs.center), half_axs_length(rhs.half_axs_length), ratio(rhs.ratio) {}

        // 移动构造函数
        dim2_parameterized_ellipse(dim2_parameterized_ellipse &&rhs) noexcept
            : center(std::move(rhs.center)), half_axs_length(std::move(rhs.half_axs_length)), ratio(std::move(rhs.ratio)) {}

        // 拷贝赋值运算符
        dim2_parameterized_ellipse &operator=(const dim2_parameterized_ellipse &rhs) {
            if (this == &rhs) return *this;
            center = rhs.center;
            half_axs_length = rhs.half_axs_length;
            ratio = rhs.ratio;
            return *this;
        }

        // 移动赋值运算符
        dim2_parameterized_ellipse &operator=(dim2_parameterized_ellipse &&rhs) noexcept {
            if (this == &rhs) return *this;
            center = std::move(rhs.center);
            half_axs_length = std::move(rhs.half_axs_length);
            ratio = std::move(rhs.ratio);
            return *this;
        }

        // 析构函数
        ~dim2_parameterized_ellipse() = default;

        [[nodiscard]] vector2Dr get_param_range() const override { return {0, ratio(0) / ratio(1)}; } // 获取参数范围
        [[nodiscard]] vector2Dr get_plot_ratio() const override { return ratio; }                     // 获取绘图比例
        [[nodiscard]] vector2Dr get_line_value(real_number_cref t) const override;                    // 根据参数 t 获取曲线上点
        [[nodiscard]] para_type type() const override { return para_type::ellipse; }                  // 获取曲线类型
    };
    class dim2_parameterized_arc : public dim2_parameterized_dim1 {
        vector2Dr center;
        real_number radius;
        vector2Dr angle_from_to;
        vector2Dr ratio;

     public:
        // 默认构造函数
        dim2_parameterized_arc() : center {0, 0}, radius {1}, angle_from_to {0, 1}, ratio {1, 1} {}

        // 带参数的构造函数
        dim2_parameterized_arc(vector2Dr center_, real_number radius_, vector2Dr angle_from_to_, vector2Dr ratio_ = {1, 1})
            : center(std::move(center_)), radius(radius_), angle_from_to(std::move(angle_from_to_)), ratio(std::move(ratio_)) {}

        // 拷贝构造函数
        dim2_parameterized_arc(const dim2_parameterized_arc &rhs)
            : center(rhs.center), radius(rhs.radius), angle_from_to(rhs.angle_from_to), ratio(rhs.ratio) {}

        // 移动构造函数
        dim2_parameterized_arc(dim2_parameterized_arc &&rhs) noexcept
            : center(std::move(rhs.center)), radius(rhs.radius), angle_from_to(std::move(rhs.angle_from_to)), ratio(std::move(rhs.ratio)) {}

        // 拷贝赋值运算符
        dim2_parameterized_arc &operator=(const dim2_parameterized_arc &rhs) {
            if (this == &rhs) return *this;
            center = rhs.center;
            radius = rhs.radius;
            angle_from_to = rhs.angle_from_to;
            ratio = rhs.ratio;
            return *this;
        }

        // 移动赋值运算符
        dim2_parameterized_arc &operator=(dim2_parameterized_arc &&rhs) noexcept {
            if (this == &rhs) return *this;
            center = std::move(rhs.center);
            radius = rhs.radius;
            angle_from_to = std::move(rhs.angle_from_to);
            ratio = std::move(rhs.ratio);
            return *this;
        }

        // 析构函数
        ~dim2_parameterized_arc() = default;

        [[nodiscard]] vector2Dr get_param_range() const override { return {0, ratio(0) / ratio(1)}; } // 获取参数范围
        [[nodiscard]] vector2Dr get_plot_ratio() const override { return ratio; }                     // 获取绘图比例
        [[nodiscard]] vector2Dr get_line_value(real_number_cref t) const override;                    // 根据参数 t 获取曲线上点
        [[nodiscard]] para_type type() const override { return para_type::arc; }                      // 获取曲线类型
    };
    class dim2_parameterized_elliarc : dim2_parameterized_dim1 {
        vector2Dr center;          // 椭圆中心坐标
        vector2Dr half_axs_length; // 半轴长度
        vector2Dr angle_from_to;   // 起始和终止角度
        vector2Dr ratio;           // 比例因子

     public:
        // 默认构造函数
        dim2_parameterized_elliarc() : center {0, 0}, half_axs_length {1, 1}, angle_from_to {0, 1}, ratio {1, 1} {}

        // 带参数的构造函数
        dim2_parameterized_elliarc(vector2Dr center_, vector2Dr half_axs_length_, vector2Dr angle_from_to_, vector2Dr ratio_ = {1, 1})
            : center(std::move(center_)), half_axs_length(std::move(half_axs_length_)), angle_from_to(std::move(angle_from_to_)), ratio(std::move(ratio_)) {}

        // 拷贝构造函数
        dim2_parameterized_elliarc(const dim2_parameterized_elliarc &rhs)
            : center(rhs.center), half_axs_length(rhs.half_axs_length), angle_from_to(rhs.angle_from_to), ratio(rhs.ratio) {}

        // 移动构造函数
        dim2_parameterized_elliarc(dim2_parameterized_elliarc &&rhs) noexcept
            : center(std::move(rhs.center)), half_axs_length(std::move(rhs.half_axs_length)), angle_from_to(std::move(rhs.angle_from_to)), ratio(std::move(rhs.ratio)) {}

        // 拷贝赋值运算符
        dim2_parameterized_elliarc &operator=(const dim2_parameterized_elliarc &rhs) {
            if (this == &rhs) return *this;
            center = rhs.center;
            half_axs_length = rhs.half_axs_length;
            angle_from_to = rhs.angle_from_to;
            ratio = rhs.ratio;
            return *this;
        }

        // 移动赋值运算符
        dim2_parameterized_elliarc &operator=(dim2_parameterized_elliarc &&rhs) noexcept {
            if (this == &rhs) return *this;
            center = std::move(rhs.center);
            half_axs_length = std::move(rhs.half_axs_length);
            angle_from_to = std::move(rhs.angle_from_to);
            ratio = std::move(rhs.ratio);
            return *this;
        }

        // 析构函数
        ~dim2_parameterized_elliarc() = default;

        [[nodiscard]] vector2Dr get_param_range() const override { return {0, ratio(0) / ratio(1)}; } // 获取参数范围
        [[nodiscard]] vector2Dr get_plot_ratio() const override { return ratio; }                     // 获取绘图比例
        [[nodiscard]] vector2Dr get_line_value(real_number_cref t) const override;                    // 根据参数 t 获取曲线上点
        [[nodiscard]] para_type type() const override { return para_type::elliarc; }                  // 获取曲线类型
    };
    class dim2_parameterized_quad_bezier : public dim2_parameterized_dim1 {
        std::optional<vector2Dr> from;
        std::optional<vector2Dr> ctrl;
        vector2Dr to;
        vector2Dr ratio;

     public:
        // 默认构造函数
        dim2_parameterized_quad_bezier()
            : from(vector2Dr {0, 0}), ctrl(vector2Dr {0.5, 0.5}), to(vector2Dr {1, 1}), ratio(vector2Dr {1, 1}) {}

        // 带参数的构造函数
        dim2_parameterized_quad_bezier(vector2Dr from_, vector2Dr control_, vector2Dr to_, vector2Dr ratio_ = {1, 1})
            : from(std::move(from_)),
              ctrl(std::move(control_)),
              to(std::move(to_)),
              ratio(std::move(ratio_)) {}

        // 拷贝构造函数
        dim2_parameterized_quad_bezier(const dim2_parameterized_quad_bezier &rhs)
            : from(rhs.from), ctrl(rhs.ctrl), to(rhs.to), ratio(rhs.ratio) {}

        // 移动构造函数
        dim2_parameterized_quad_bezier(dim2_parameterized_quad_bezier &&rhs) noexcept
            : from(std::move(rhs.from)),
              ctrl(std::move(rhs.ctrl)),
              to(std::move(rhs.to)),
              ratio(std::move(rhs.ratio)) {}

        // 拷贝赋值运算符
        dim2_parameterized_quad_bezier &operator=(const dim2_parameterized_quad_bezier &rhs) {
            if (this == &rhs)
                return *this;
            from = rhs.from;
            ctrl = rhs.ctrl;
            to = rhs.to;
            ratio = rhs.ratio;
            return *this;
        }

        // 移动赋值运算符
        dim2_parameterized_quad_bezier &operator=(dim2_parameterized_quad_bezier &&rhs) noexcept {
            if (this == &rhs)
                return *this;
            from = std::move(rhs.from);
            ctrl = std::move(rhs.ctrl);
            to = std::move(rhs.to);
            ratio = std::move(rhs.ratio);
            return *this;
        }

        // 析构函数
        ~dim2_parameterized_quad_bezier() = default;

        [[nodiscard]] vector2Dr get_param_range() const override { return {0, ratio(0) / ratio(1)}; } // 获取参数范围
        [[nodiscard]] vector2Dr get_plot_ratio() const override { return ratio; }                     // 获取绘图比例
        [[nodiscard]] vector2Dr get_line_value(real_number_cref t) const override;                    // 根据参数 t 获取曲线上点
        [[nodiscard]] para_type type() const override { return para_type::quad_bezier; }              // 获取曲线类型
    };
    class dim2_parameterized_cube_bezier : public dim2_parameterized_dim1 {
        std::optional<vector2Dr> from;
        std::optional<vector2Dr> ctrl_f;
        vector2Dr ctrl_t;
        vector2Dr to;
        vector2Dr ratio;

     public:
        // 默认构造函数
        dim2_parameterized_cube_bezier()
            : from(vector2Dr {0, 0}),
              ctrl_f(vector2Dr {0.33, 0.33}),
              ctrl_t(vector2Dr {0.66, 0.66}),
              to(vector2Dr {1, 1}),
              ratio(vector2Dr {1, 1}) {}

        // 带参数的构造函数
        dim2_parameterized_cube_bezier(vector2Dr from_, vector2Dr ctrl_f, vector2Dr ctrl_t, vector2Dr to_, vector2Dr ratio_ = {1, 1})
            : from(std::move(from_)),
              ctrl_f(std::move(ctrl_f)),
              ctrl_t(std::move(ctrl_t)),
              to(std::move(to_)),
              ratio(std::move(ratio_)) {}

        // 拷贝构造函数
        dim2_parameterized_cube_bezier(const dim2_parameterized_cube_bezier &rhs)
            : from(rhs.from),
              ctrl_f(rhs.ctrl_f),
              ctrl_t(rhs.ctrl_t),
              to(rhs.to),
              ratio(rhs.ratio) {}

        // 移动构造函数
        dim2_parameterized_cube_bezier(dim2_parameterized_cube_bezier &&rhs) noexcept
            : from(std::move(rhs.from)),
              ctrl_f(std::move(rhs.ctrl_f)),
              ctrl_t(std::move(rhs.ctrl_t)),
              to(std::move(rhs.to)),
              ratio(std::move(rhs.ratio)) {}

        // 拷贝赋值运算符
        dim2_parameterized_cube_bezier &operator=(const dim2_parameterized_cube_bezier &rhs) {
            if (this == &rhs)
                return *this;
            from = rhs.from;
            ctrl_f = rhs.ctrl_f;
            ctrl_t = rhs.ctrl_t;
            to = rhs.to;
            ratio = rhs.ratio;
            return *this;
        }

        // 移动赋值运算符
        dim2_parameterized_cube_bezier &operator=(dim2_parameterized_cube_bezier &&rhs) noexcept {
            if (this == &rhs)
                return *this;
            from = std::move(rhs.from);
            ctrl_f = std::move(rhs.ctrl_f);
            ctrl_t = std::move(rhs.ctrl_t);
            to = std::move(rhs.to);
            ratio = std::move(rhs.ratio);
            return *this;
        }

        // 析构函数
        ~dim2_parameterized_cube_bezier() = default;

        [[nodiscard]] vector2Dr get_param_range() const override { return {0, ratio(0) / ratio(1)}; } // 获取参数范围
        [[nodiscard]] vector2Dr get_plot_ratio() const override { return ratio; }                     // 获取绘图比例
        [[nodiscard]] vector2Dr get_line_value(real_number_cref t) const override;                    // 根据参数 t 获取曲线上点
        [[nodiscard]] para_type type() const override { return para_type::cube_bezier; }              // 获取曲线类型
    };


    // 绘图元素的基类
    class line_shape_element {
     public:
        virtual ~line_shape_element() = default;

        virtual ptr_s<dim2_parameterized_dim1> operator[](size_t index) = 0; // 下标运算符，获取指定索引的元素
        virtual size_t size() = 0;                                           // 获取元素数量的纯虚函数
        virtual bool empty() = 0;                                            // 判断是否为空的纯虚函数
        virtual bool good() = 0;                                             // 判断状态是否正常的纯虚函数
        virtual int type() = 0;                                              // 获取类型的纯虚函数
    };
    class line_shape_rect : public line_shape_element {};     // 矩形元素类
    class line_shape_circle : public line_shape_element {};   // 圆形元素类
    class line_shape_ellipse : public line_shape_element {};  // 椭圆元素类
    class line_shape_line : public line_shape_element {};     // 直线元素类
    class line_shape_polyline : public line_shape_element {}; // 折线元素类
    class line_shape_polygon : public line_shape_element {};  // 多边形元素类
    class line_shape_path : public line_shape_element {};     // 路径元素类

    namespace _details
    {
        using para_type = dim2_parameterized_dim1::para_type;
#define DIM2_USING(X) using dim2_parameterized_dim1::para_type::X
        DIM2_USING(error);
        DIM2_USING(line);
        DIM2_USING(circle);
        DIM2_USING(ellipse);
        DIM2_USING(arc);
        DIM2_USING(elliarc);
        DIM2_USING(quad_bezier);
        DIM2_USING(cube_bezier);
#undef DIM2_USING

        template <class T>
        struct param_type_number : std::integral_constant<para_type, error> {
            using _use_type = T;
        };
#define PARAM_TYPE_NUMBER(X, Y)                                          \
    template <>                                                          \
    struct param_type_number<X> : std::integral_constant<para_type, Y> { \
        using _use_type = X;                                             \
    }
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
