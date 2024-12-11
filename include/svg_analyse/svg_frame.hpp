#pragma once // 防止头文件被重复包含

#include <utility>               // 引入标准库的 utility 头文件，提供工具函数
#include "../basic_particle.hpp" // 引入相对路径的 basic_particle.hpp 头文件

namespace mc_particle::svg // 定义命名空间 mc_particle::svg
{
    // 可一维参数化的二维对象的基类
    class dim2_parameterized_dim1 {
     public:
        enum class para_type {
            line,        // 直线
            circle,      // 圆
            ellipse,     // 椭圆
            arc,         // 圆弧
            elliarc,     // 椭圆弧
            quad_bezier, // 二次贝塞尔曲线
            cube_bezier  // 三次贝塞尔曲线
        };
        [[nodiscard]] virtual vector2Dr get_param_range() const = 0;                                       // 获取参数范围的纯虚函数
        [[nodiscard]] virtual vector2Dr get_plot_ratio() const = 0;                                        // 获取绘图比例的纯虚函数
        [[nodiscard]] virtual vector2Dr operator()(real_number_cref t) const { return get_line_value(t); } // 重载()运算符，方便调用
        [[nodiscard]] virtual vector2Dr get_line_value(real_number_cref t) const = 0;                      // 根据参数 t 获取曲线上点的纯虚函数
        [[nodiscard]] virtual para_type type() const = 0;                                                  // 获取曲线类型的纯虚函数

        virtual ~dim2_parameterized_dim1() = default;
    };

    // 参数化的直线类，继承自 dim2_parameterized_dim1
    class dim2_parameterized_line : dim2_parameterized_dim1 {
        vector2Dr from;  // 起点坐标
        vector2Dr to;    // 终点坐标
        vector2Dr ratio; // 比例因子

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
    class dim2_parameterized_circle : dim2_parameterized_dim1 {
        vector2Dr center;   // 圆心坐标
        real_number radius; // 半径
        vector2Dr ratio;    // 比例因子

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
        ~dim2_parameterized_circle() override = default;
        // 重写基类的纯虚函数
        [[nodiscard]] vector2Dr get_param_range() const override { return {0, ratio(0) / ratio(1)}; } // 获取参数范围
        [[nodiscard]] vector2Dr get_plot_ratio() const override { return ratio; }                     // 获取绘图比例
        [[nodiscard]] vector2Dr get_line_value(real_number_cref t) const override;                    // 根据参数 t 获取曲线上点
        [[nodiscard]] para_type type() const override { return para_type::circle; }                   // 获取曲线类型

        // 个性化的成员函数
        // TODO
    };
    class dim2_parameterized_ellipse : dim2_parameterized_dim1 {
        vector2Dr center;          // 椭圆中心坐标
        vector2Dr half_axs_length; // 半轴长度
        vector2Dr ratio;           // 比例因子

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
        ~dim2_parameterized_ellipse() override = default;

        [[nodiscard]] vector2Dr get_param_range() const override { return {0, ratio(0) / ratio(1)}; } // 获取参数范围
        [[nodiscard]] vector2Dr get_plot_ratio() const override { return ratio; }                     // 获取绘图比例
        [[nodiscard]] vector2Dr get_line_value(real_number_cref t) const override;                    // 根据参数 t 获取曲线上点
        [[nodiscard]] para_type type() const override { return para_type::ellipse; }                  // 获取曲线类型
    };
    class dim2_parameterized_arc : dim2_parameterized_dim1 {
        vector2Dr center;        // 圆心坐标
        real_number radius;      // 半径
        vector2Dr angle_from_to; // 起始和终止角度
        vector2Dr ratio;         // 比例因子

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
        ~dim2_parameterized_arc() override = default;

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
        ~dim2_parameterized_elliarc() override = default;

        [[nodiscard]] vector2Dr get_param_range() const override { return {0, ratio(0) / ratio(1)}; } // 获取参数范围
        [[nodiscard]] vector2Dr get_plot_ratio() const override { return ratio; }                     // 获取绘图比例
        [[nodiscard]] vector2Dr get_line_value(real_number_cref t) const override;                    // 根据参数 t 获取曲线上点
        [[nodiscard]] para_type type() const override { return para_type::elliarc; }                  // 获取曲线类型
    };
    class dim2_parameterized_quad_bezier : dim2_parameterized_dim1 {
        vector2Dr from;  // 起点
        vector2Dr ctrl;  // 控制点
        vector2Dr to;    // 终点
        vector2Dr ratio; // 比例因子

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
        ~dim2_parameterized_quad_bezier() override = default;

        [[nodiscard]] vector2Dr get_param_range() const override { return {0, ratio(0) / ratio(1)}; } // 获取参数范围
        [[nodiscard]] vector2Dr get_plot_ratio() const override { return ratio; }                     // 获取绘图比例
        [[nodiscard]] vector2Dr get_line_value(real_number_cref t) const override;                    // 根据参数 t 获取曲线上点
        [[nodiscard]] para_type type() const override { return para_type::quad_bezier; }              // 获取曲线类型
    };
    class dim2_parameterized_cube_bezier : dim2_parameterized_dim1 {
        vector2Dr from;   // 起点
        vector2Dr ctrl_f; // 第一个控制点
        vector2Dr ctrl_t; // 第二个控制点
        vector2Dr to;     // 终点
        vector2Dr ratio;  // 比例因子

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
        ~dim2_parameterized_cube_bezier() override = default;

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


} // namespace mc_particle::svg
