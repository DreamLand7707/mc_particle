# linear_particle __类__

### 参数说明

real_number error = 1e-6;误差
integer_number length = 0;不用管
integer_number prec = 6;不用管

### 指令示例

particleex tickparameter <颗粒名> <x> <y> <z> <红> <绿> <蓝> <透明度> <亮度> <vx> <vy> <vz> <tStart> <tEnd> <表达式> [计算间隔] [CPT] [寿命] [速度表达式] [计算间隔] [组] [参数]

### 调用函数

solid_color_static_sequential(
            const particle_tick_parameter_options &opt,
            const vector3D<real_number> &from, const vector3D<real_number> &to) const;

### 参考代码

​    particle_basic_options basic_test;

    parameter_particle_options para_part({0.0, 10.0}, 100, 0.1, false,
                                         basic_test);
    
    para_part.set_t_begin_end(para_part.confirm_t_begin_end());
    
    tick_parameter_particle_options tick_para(10, para_part);
    
    particle_tick_parameter_options part_tick({0.9, 0.2, 0.7, 1.0}, tick_para);
    
    particle_rgba_tick_parameter_options part_rgba(tick_para);
    
    str parameter_expr = "x,y,z=t,sin(t),t";
    
    str velocity_expr = "(vx,,vy,,vz)=(1,0,0,,0,1,0,,0,0,1)*(x,,y,,z))";
    
    linear_particle linear_part;
    
    vector3D<real_number> from = {-241, 4, -197};  // 起点位置
    vector3D<real_number> to = {-247, 4, -197};    // 终点位置
    
    str result = linear_part.solid_color_static_sequential(part_tick, from, to);

### 结果示例

particleex tickparameter minecraft:end_rod 0.000000 0.000000 0.000000 0.900000 0.200000 0.700000 1.000000 0.000000 0.000000 0.000000 0.000000 9.950000 "x = (-241.000000) + (-0.606061) * t; y = (5.000000) + (-0.000000) * t; z = (-197.000000) + (-0.000000) * t; " 0.100000 10 0 "null" 1.000000

### 调用函数

str solid_color_static_instantaneous(
            const particle_parameter_options &opt,
            const vector3D<real_number> &from, const vector3D<real_number> &to) const;

### 参考代码

​    particle_basic_options basic_test;

    parameter_particle_options para_part({0.0, 10.0}, 100, 0.1, false,
                                         basic_test);
    
    para_part.set_t_begin_end(para_part.confirm_t_begin_end());
    
    tick_parameter_particle_options tick_para(10, para_part);
    
    particle_tick_parameter_options part_tick({0.9, 0.2, 0.7, 1.0}, tick_para);
    
    particle_parameter_options part_para({0.9, 0.2, 0.7, 1.0}, para_part);
    
    str parameter_expr = "x,y,z=t,sin(t),t";
    
    str velocity_expr = "(vx,,vy,,vz)=(1,0,0,,0,1,0,,0,0,1)*(x,,y,,z))";
    
    linear_particle linear_part;
    
    vector3D<real_number> from = {-241, 4, -197};  // 起点位置
    vector3D<real_number> to = {-247, 4, -197};    // 终点位置
    
    str result =
        linear_part.solid_color_static_instantaneous(part_para, from, to);
    
    CopyTextToClipboard(result);

### 结果示例

particleex parameter minecraft:end_rod 0.000000 0.000000 0.000000 0.900000 0.200000 0.700000 1.000000 0.000000 0.000000 0.000000 0.000000 9.950000 "x = (-241.000000) + (-0.606061) * t; y = (4.000000) + (-0.000000) * t; z = (-197.000000) + (-0.000000) * t; " 0.100000 0 "null" 1.000000



比较上二者不同
首先要明确：
<计算间隔> 按照表达式控制粒子时自变量每次递增的值
<tStart> <tEnd>或<begin> <end> 当表达式为参数方程时t的初始值和最终值 (终值>初值)
<CPT> 在带有tick的子命令中控制每tick执行<表达式>的次数
其中solid_color_static_instantaneous是立刻生成图形（CPI约为正无穷）
solid_color_static_sequential是渐渐生成图形。

### 调用函数

str gradual_color_static_sequential(
            const particle_rgba_tick_parameter_options &opt,
            const vector3D<real_number> &from, const vector3D<real_number> &to,
            Input_it iter_begin, Input_it iter_end) const;

### 参考程序：

​    color_position a{0.1, {0.9, 0.2, 0.7, 1.0}};
​    color_position b{0.7, {0.2, 0.9, 0.7, 1.0}};
​    std::vector<color_position> color_positions = {a, b};

    particle_basic_options basic_test;
    
    parameter_particle_options para_part({0.0, 10.0}, 100, 0.1, false,
                                         basic_test);
    
    para_part.set_t_begin_end(para_part.confirm_t_begin_end());
    
    tick_parameter_particle_options tick_para(10, para_part);
    
    particle_tick_parameter_options part_tick({0.9, 0.2, 0.7, 1.0}, tick_para);
    
    particle_parameter_options part_para({0.9, 0.2, 0.7, 1.0}, para_part);
    
    particle_rgba_tick_parameter_options part_rgba_tick(tick_para);
    
    str parameter_expr = "x,y,z=t,sin(t),t";
    
    str velocity_expr = "(vx,,vy,,vz)=(1,0,0,,0,1,0,,0,0,1)*(x,,y,,z))";
    
    linear_particle linear_part;
    
    vector3D<real_number> from = {-241, 4, -197};  // 起点位置
    vector3D<real_number> to = {-247, 4, -197};    // 终点位置
    
    str result = linear_part.gradual_color_static_sequential(
        part_rgba_tick, from, to, color_positions.begin(),
        color_positions.end());

### 参考结果

particleex rgbatickparameter minecraft:end_rod 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 9.950000 " x = (-241.000000) + (-0.606061) * t;  y = (4.000000) + (-0.000000) * t;  z = (-197.000000) + (-0.000000) * t;  cr = ((t<(0.990000))*(0.900000)) + (((t>=(0.990000))&(t<(6.930000)))*((-0.117845)*t+(1.016667))) + ((t>=(6.930000))*(0.200000));  cg = ((t<(0.990000))*(0.200000)) + (((t>=(0.990000))&(t<(6.930000)))*((0.117845)*t+(0.083333))) + ((t>=(6.930000))*(0.900000));  cb = ((t<(0.990000))*(0.700000)) + (((t>=(0.990000))&(t<(6.930000)))*((-0.000000)*t+(0.700000))) + ((t>=(6.930000))*(0.700000));  alpha = ((t<(0.990000))*(1.000000)) + (((t>=(0.990000))&(t<(6.930000)))*((-0.000000)*t+(1.000000))) + ((t>=(6.930000))*(1.000000)); " 0.100000 10 0 "null" 1.000000

### 调用函数

str solid_color_static_spiral_sequential(
            const particle_tick_parameter_options &opt,
            const vector3D<real_number> &from, const vector3D<real_number> &to,
            bool clockwise,
            real_number_cref rad_bias,
            real_number_cref circles,
            real_number_cref radius) const;

### 函数内各个名称的意义

​    clockwise：不知道，暂时搁置
​    rad_bias：初始偏置
​    circles：自始至终的圆圈数
​    radius：半径

### 参考程序

​    particle_basic_options basic_test;

    parameter_particle_options para_part({0.0, 10.0}, 1000, 0.1, false,
                                         basic_test);
    
    para_part.set_t_begin_end(para_part.confirm_t_begin_end());
    
    tick_parameter_particle_options tick_para(10, para_part);
    
    particle_tick_parameter_options part_tick({0.9, 0.2, 0.7, 1.0}, tick_para);
    
    particle_parameter_options part_para({0.9, 0.2, 0.7, 1.0}, para_part);
    
    particle_rgba_tick_parameter_options part_rgba_tick(tick_para);
    
    str parameter_expr = "x,y,z=t,sin(t),t";
    
    str velocity_expr = "(vx,,vy,,vz)=(1,0,0,,0,1,0,,0,0,1)*(x,,y,,z))";
    
    linear_particle linear_part;
    
    vector3D<real_number> from = {-241, 10, -197};  // 起点位置
    vector3D<real_number> to = {-247, 10, -197};    // 终点位置
    
    str result = linear_part.solid_color_static_spiral_sequential(part_tick,from,to,true,0,6,5);
    
    CopyTextToClipboard(result);

### 参考结果

particleex tickparameter minecraft:end_rod 0.000000 0.000000 0.000000 0.900000 0.200000 0.700000 1.000000 0.000000 0.000000 0.000000 0.000000 99.950000 " x = (0.000000)*(((-(5.000000))*(sin((0.377368)*(t+(0.000000))+(0.000000)))))+(-1.000000)*(((0.060060)*t+(0.000000)))+(0.000000)*(((5.000000)*(cos((0.377368)*(t+(0.000000))+(0.000000)))))+(-241.000000); y = (1.000000)*(((-(5.000000))*(sin((0.377368)*(t+(0.000000))+(0.000000)))))+(0.000000)*(((0.060060)*t+(0.000000)))+(0.000000)*(((5.000000)*(cos((0.377368)*(t+(0.000000))+(0.000000)))))+(10.000000); z = (0.000000)*(((-(5.000000))*(sin((0.377368)*(t+(0.000000))+(0.000000)))))+(0.000000)*(((0.060060)*t+(0.000000)))+(-1.000000)*(((5.000000)*(cos((0.377368)*(t+(0.000000))+(0.000000)))))+(-197.000000);" 0.100000 10 0 "null" 1.000000

### 参考函数

str solid_color_expand_spiral_instantaneous_file(
            const particle_parameter_options &opt,
            const mc_function &target_file,
            const vector3D<real_number> &from,
            const vector3D<real_number> &to,
            bool clockwise,
            real_number_cref rad_bias,
            real_number_cref circles,
            real_number_cref radius,
            real_number_cref resistence) const;

### 函数内各个名称的意义

​    resistence 粒子扩散速度（负相关）

### 参考程序

​    namespace bf = boost::filesystem;

    datapack dp("F:/360MoveData/Users/hao123/Desktop/123/MC/.minecraft/versions/1.16.5-Fabric 0.14.9-OptiFine_G8/saves/新的世界/datapacks", "my_datapack");
    dp_namespace ns = dp.create_namespace("example");
    mc_function func = ns.create_mc_function("", "test_func");
    
    if (!dp.create_fs())
        std::cout << "Error: Can't Create DataPack!" << std::endl;
    if (!ns.create_fs())
        std::cout << "Error: Can't Create Namespace!" << std::endl;
    if (!func.create_fs())
        std::cout << "Error: Can't Create Function!" << std::endl;
    
    particle_parameter_options para_opti(
        {1.0, 1.0, 1.0, 1.0},
        parameter_particle_options(
            {0.0, 10.0},
            200,
            0.05,
            false,
            particle_basic_options()));
    
    para_opti.set_para_counting_period(0.01);
    para_opti.confirm_particle_num();
    
    linear_particle lp;
    
    vector3D<real_number> points[] = {
        {-241, 10, -197}, {184, 100, -215}, {-247, 10, -197}};
    
    std::cout << lp.solid_color_expand_spiral_instantaneous_file(para_opti, func,
                                                                 points[0], points[2], true, 0, 6,
                                                                 5, 0.2);

### 参考结果

​    请查看mcfunction使用指南

### 参考函数

str gradual_color_static_spiral_sequential(
            const particle_rgba_tick_parameter_options &opt,
            const vector3D<real_number> &from, const vector3D<real_number> &to,
            bool clockwise, real_number_cref rad_bias, real_number_cref circles,
            real_number_cref radius, Input_it iter_begin,
            Input_it iter_end) const;

### 函数内各个名称的意义

​    from to:出发地和目的地的坐标
​    clockwise:顺时针还是逆时针
​    rad_bias:起始弧度的偏置
​    circles:圆圈数
​    radius:半径
​    iter_begin：迭代器起始值
​    iter_end: 迭代器末尾值

### 参考程序

​    color_position a{0.1, {0.9, 0.2, 0.7, 1.0}};
​    color_position b{0.7, {0.2, 0.9, 0.7, 1.0}};
​    std::vector<color_position> color_positions = {a, b};

    particle_basic_options basic_test;
    
    basic_test.age=-1;
    
    parameter_particle_options para_part({0.0, 10.0}, 10000, 0.1, false,
                                         basic_test);
    
    para_part.set_t_begin_end(para_part.confirm_t_begin_end());
    
    tick_parameter_particle_options tick_para(1000, para_part);
    
    particle_tick_parameter_options part_tick({0.9, 0.2, 0.7, 1.0}, tick_para);
    
    particle_parameter_options part_para({0.9, 0.2, 0.7, 1.0}, para_part);
    
    particle_rgba_tick_parameter_options part_rgba_tick(tick_para);
    
    particle_rgba_parameter_options part_rgba_para(para_part);
    
    str parameter_expr = "x,y,z=t,sin(t),t";
    
    str velocity_expr = "(vx,,vy,,vz)=(1,0,0,,0,1,0,,0,0,1)*(x,,y,,z))";
    
    linear_particle linear_part;
    
    vector3D<real_number> from = {-241, 10, -197};  // 起点位置
    vector3D<real_number> to = {-247, 10, -197};    // 终点位置
    
    str result = linear_part.gradual_color_static_spiral_sequential(
        part_rgba_tick, from, to, true, 0, 4, 5, color_positions.begin(),
        color_positions.end());

### 参考结果

​    particleex rgbatickparameter minecraft:end_rod 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 999.950000 " x = (0.000000)*(((-(5.000000))*(sin((0.025135)*(t+(0.000000))+(0.000000)))))+(-1.000000)*(((0.006001)*t+(0.000000)))+(0.000000)*(((5.000000)*(cos((0.025135)*(t+(0.000000))+(0.000000)))))+(-241.000000); y = (1.000000)*(((-(5.000000))*(sin((0.025135)*(t+(0.000000))+(0.000000)))))+(0.000000)*(((0.006001)*t+(0.000000)))+(0.000000)*(((5.000000)*(cos((0.025135)*(t+(0.000000))+(0.000000)))))+(10.000000); z = (0.000000)*(((-(5.000000))*(sin((0.025135)*(t+(0.000000))+(0.000000)))))+(0.000000)*(((0.006001)*t+(0.000000)))+(-1.000000)*(((5.000000)*(cos((0.025135)*(t+(0.000000))+(0.000000)))))+(-197.000000); cr = ((t<(99.990000))*(0.900000)) + (((t>=(99.990000))&(t<(699.930000)))*((-0.001167)*t+(1.016667))) + ((t>=(699.930000))*(0.200000));  cg = ((t<(99.990000))*(0.200000)) + (((t>=(99.990000))&(t<(699.930000)))*((0.001167)*t+(0.083333))) + ((t>=(699.930000))*(0.900000));  cb = ((t<(99.990000))*(0.700000)) + (((t>=(99.990000))&(t<(699.930000)))*((-0.000000)*t+(0.700000))) + ((t>=(699.930000))*(0.700000));  alpha = ((t<(99.990000))*(1.000000)) + (((t>=(99.990000))&(t<(699.930000)))*((-0.000000)*t+(1.000000))) + ((t>=(699.930000))*(1.000000)); " 0.100000 1000 -1 "null" 1.000000

### 参考函数

str gradual_color_expand_spiral_instantaneous_file(
            const particle_parameter_options &opt,
            const mc_function &target_file, const vector3D<real_number> &from,
            const vector3D<real_number> &to, bool clockwise,
            real_number_cref rad_bias, real_number_cref circles,
            real_number_cref radius, real_number_cref resistence,
            Input_it iter_begin, Input_it iter_end) const;

### 参考程序

​    using namespace mc_particle;
​    namespace bf = boost::filesystem;

    datapack dp(
        "前面省略/.minecraft/versions/"
        "1.16.5-Fabric 0.14.9-OptiFine_G8/saves/test_hy258/datapacks",
        "my_datapack");
    dp_namespace ns = dp.create_namespace("example");
    mc_function func = ns.create_mc_function("", "test_func");
    
    color_position a{0.1, {0.9, 0.2, 0.7, 1.0}};
    color_position b{0.4, {0.2, 0.9, 0.7, 1.0}};
    color_position c{0.7, {0.7, 0.7, 0.2, 1.0}};
    color_position d{0.9, {0.2, 0.7, 0.9, 1.0}};
    std::vector<color_position> color_positions = {a, b, c, d};
    
    particle_basic_options basic_test;
    
    basic_test.age = 100;
    
    parameter_particle_options para_part({0.0, 10}, 1000, 0.1, false,
                                         basic_test);
    
    para_part.set_t_begin_end(para_part.confirm_t_begin_end());
    
    particle_parameter_options part_para({0.9, 0.2, 0.7, 1.0}, para_part);
    
    linear_particle linear_part;
    
    vector3D<real_number> from = {83, 10, 57};  // 起点位置
    vector3D<real_number> to = {83, 10, 51};    // 终点位置
    
    str result = linear_part.gradual_color_expand_spiral_instantaneous_file(
        part_para, func, from, to, true, 0, 5, 5, 0.1, color_positions.begin(),
        color_positions.end());
    
    CopyTextToClipboard(result);

### 参考结果

​    加入色彩渐变

### 参考函数

str linear_particle::gradual_color_expand_spiral_sequential_file(
        const particle_parameter_options &opt, const mc_function &target_file,
        const vector3D<real_number> &from, const vector3D<real_number> &to,
        bool clockwise, real_number_cref rad_bias, real_number_cref circles,
        real_number_cref radius, real_number_cref resistence, Input_it iter_begin,
        Input_it iter_end) const

### 参考程序

using namespace mc_particle;
    namespace bf = boost::filesystem;

    datapack dp(
        "F:/360MoveData/Users/hao123/Desktop/123/MC/.minecraft/versions/"
        "1.16.5-Fabric 0.14.9-OptiFine_G8/saves/test_hy258/datapacks",
        "my_datapack");
    dp_namespace ns = dp.create_namespace("example");
    mc_function func = ns.create_mc_function("", "test_func");
    
    color_position a{0.1, {0.9, 0.2, 0.7, 1.0}};
    color_position b{0.4, {0.2, 0.9, 0.7, 1.0}};
    color_position c{0.7, {0.7, 0.7, 0.2, 1.0}};
    color_position d{0.9, {0.2, 0.7, 0.9, 1.0}};
    std::vector<color_position> color_positions = {a, b, c, d};
    
    particle_basic_options basic_test;
    
    basic_test.age = 10000;
    basic_test.velt_counting_period = 1;
    
    parameter_particle_options para_part({0.0, 10}, 1001, 0.1, false,
                                         basic_test);
    
    para_part.set_t_begin_end(para_part.confirm_t_begin_end());
    
    particle_parameter_options part_para({0.9, 0.2, 0.7, 1.0}, para_part);
    
    linear_particle linear_part;
    
    vector3D<real_number> from = {83, 10, 57};  // 起点位置
    vector3D<real_number> to = {83, 10, 51};    // 终点位置
    
    str result = linear_part.gradual_color_expand_spiral_sequential_file(
        part_para, func, from, to, true, 0, 5, 5, 0.1, color_positions.begin(),
        color_positions.end());

### 参考结果

​    是一个中间有直线的，向外扩散的圆。中间没有直线的圆还没做。



### 推荐教程


https://eigen.tuxfamily.org/dox/modules.html
Dense matrix and array manipulation
Dense linear problems and decompositions
