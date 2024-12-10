#pragma once
#include <utility>

#include "../basic_particle.hpp"


namespace mc_particle::svg
{
    // 可一维参数化的二维对象
    class dim2_parameterized_dim1 {
     public:
        enum class para_type {
            line,
            circle,
            ellipse,
            arc,
            elliarc,
            quad_bezier,
            cube_bezier
        };
        virtual vector2Dr get_param_range() = 0;
        virtual vector2Dr get_plot_ratio() = 0;
        virtual vector2Dr operator()(real_number_cref t) { return get_line_value(t); }
        virtual vector2Dr get_line_value(real_number_cref t) = 0;
        virtual para_type type() = 0;
    };

    class dim2_parameterized_line : dim2_parameterized_dim1 {
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
        vector2Dr get_param_range() override { return {0, ratio(0) / ratio(1)}; }
        vector2Dr get_plot_ratio() override { return ratio; }
        vector2Dr get_line_value(real_number_cref t) override;
        para_type type() override { return para_type::line; }

        // personalized
        vector2Dr get_from() { return from; }
        vector2Dr get_to() { return to; }
    };
    class dim2_parameterized_circle : dim2_parameterized_dim1 {
        vector2Dr center;
        real_number radius;
        vector2Dr ratio;

     public:
        // from base
        vector2Dr get_param_range() override { return {0, ratio(0) / ratio(1)}; }
        vector2Dr get_plot_ratio() override { return ratio; }
        vector2Dr get_line_value(real_number_cref t) override;
        para_type type() override { return para_type::circle; }

        // personalized
        // TODO
    };
    class dim2_parameterized_ellipse : dim2_parameterized_dim1 {
        vector2Dr center;
        vector2Dr half_axs_length;
        vector2Dr ratio;

     public:
        vector2Dr get_param_range() override { return {0, ratio(0) / ratio(1)}; }
        vector2Dr get_plot_ratio() override { return ratio; }
        vector2Dr get_line_value(real_number_cref t) override;
        para_type type() override { return para_type::ellipse; }
    };
    class dim2_parameterized_arc : dim2_parameterized_dim1 {
        vector2Dr center;
        real_number radius;
        vector2Dr angle_from_to;
        vector2Dr ratio;

     public:
        vector2Dr get_param_range() override { return {0, ratio(0) / ratio(1)}; }
        vector2Dr get_plot_ratio() override { return ratio; }
        vector2Dr get_line_value(real_number_cref t) override;
        para_type type() override { return para_type::arc; }
    };
    class dim2_parameterized_elliarc : dim2_parameterized_dim1 {
        vector2Dr center;
        vector2Dr half_axs_length;
        vector2Dr angle_from_to;
        vector2Dr ratio;

     public:
        vector2Dr get_param_range() override { return {0, ratio(0) / ratio(1)}; }
        vector2Dr get_plot_ratio() override { return ratio; }
        vector2Dr get_line_value(real_number_cref t) override;
        para_type type() override { return para_type::elliarc; }
    };
    class dim2_parameterized_quad_bezier : dim2_parameterized_dim1 {
        std::optional<vector2Dr> from;
        std::optional<vector2Dr> ctrl;
        vector2Dr to;
        vector2Dr ratio;

     public:
        vector2Dr get_param_range() override { return {0, ratio(0) / ratio(1)}; }
        vector2Dr get_plot_ratio() override { return ratio; }
        vector2Dr get_line_value(real_number_cref t) override;
        para_type type() override { return para_type::quad_bezier; }
    };
    class dim2_parameterized_cube_bezier : dim2_parameterized_dim1 {
        std::optional<vector2Dr> from;
        std::optional<vector2Dr> ctrl_f;
        vector2Dr ctrl_t;
        vector2Dr to;
        vector2Dr ratio;

     public:
        vector2Dr get_param_range() override { return {0, ratio(0) / ratio(1)}; }
        vector2Dr get_plot_ratio() override { return ratio; }
        vector2Dr get_line_value(real_number_cref t) override;
        para_type type() override { return para_type::cube_bezier; }
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


} // namespace mc_particle::svg
