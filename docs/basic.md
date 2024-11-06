## HeaderFile /include/basic_particle.hpp

本文档主要用于解释/include/basic_particle.hpp文件中的定义，类，函数



本文件包含以下库头文件

```c++
#include <cln/cln.h>
#include <ginac/ginac.h>
#include <Eigen/Eigen>
#include <boost/format.hpp>
#include <string>
#include <format>
#include <sstream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <cstdint>
```



在namespace mc_particle中
1. 点和向量的定义，它们的实现使用了Eigen库
    point2D point3D point4D以及相对应的vector

2. 常用数据
    整数，integer_number
    实数，real_number
    字符串，str
    字符串视图，strv
    字符串元素，单个字符，char_type
    字符流，str_stream
    文件流，f_stream

3. 为了对应游戏内指令格式，设定了如下options类
    + **particle_basic_options**
      
        基础options，所有options类的基类

        包含内容：粒子名，速度表达式计算间隔，粒子寿命

        
        
    + **particle_normal_options** -> **particle_basic_options**
      
        对应于指令的normal格式，

        包含：particle_basic_options所有内容，粒子颜色，初始速度，生成范围，生成数量

        
        
    + **particle_conditional_options** -> **particle_basic_options**
      
        对应于指令的conditional格式，

        包含：particle_basic_options所有内容，粒子颜色，初始速度，生成范围，参数计算间隔

        
        
    + **parameter_particle_options** -> **particle_basic_options**
      
        （注意parameter和particle的先后顺序）
        
        所有包含parameter的格式的基类，不对应任何一个指令
        
        包含：particle_basic_options所有内容，参数的初始和结尾，粒子数量，参数计算间隔
        
        
        
    + **particle_parameter_options** -> **parameter_particle_options**
      
        （注意parameter和particle的先后顺序）
        
        对应于指令的parameter格式
        
        包含：parameter_particle_options所有内容，颜色，初始速度
        
        
        
    + **particle_tick_parameter_options** -> **particle_parameter_options**
      
        对应于指令的tickparameter格式

        包含：particle_parameter_options所有内容，每tick计算表达式的次数

        
        
    + **particle_rgba_parameter_options** -> **parameter_particle_options**
      
        对应于指令的rgbaparameter格式

        包含：parameter_particle_options所有内容

        
        
    + **particle_rgba_tick_parameter_options** -> **particle_parameter_options**
      
        对应于指令的rgbatickparameter格式

        包含：parameter_particle_options所有内容，每tick计算表达式的次数

        
        
    + **image_particle_options** -> **particle_basic_options**
      
        （注意image和particle的先后顺序）
        
        所有包含image的格式的基类，不对应任何一个指令
        
        包含：particle_basic_options所有内容，初始速度，缩放比例，每方块粒子数
        
        
        
    + **particle_image_options** -> **image_particle_options**
      
        （注意image和particle的先后顺序）
        
        对应于指令的image格式
        
        包含：image_particle_options所有内容，x旋转角度，y旋转角度，z旋转角度，翻转
        
        
        
    + **particle_image_matrix_options** -> **image_particle_options**
      
        对应于指令的imagematrix格式
        
        包含：image_particle_options所有内容，翻转矩阵

    

    每一个options都有对应的get成员函数和set成员函数用于“获取”和“设置”属性
    options还有一个特殊的成员函数，叫做**get_option_type**()，它的作用是运行时获取options的种类，返回值如下：

    + particle_basic_options::EMPTY -> particle_basic,parameter_particle,image_particle
    + particle_basic_options::NORMAL -> particle_normal
    + particle_basic_options::CONDITIONAL -> particle_conditional
    + particle_basic_options::PARAMETER -> particle_parameter
    + particle_basic_options::TICKPARAMETER -> particle_tickparameter
    + particle_basic_options::RGBAPARAMETER -> particle_rgbaparameter
    + particle_basic_options::RGBATICKPARAMETER -> particle_rgbatickparameter
    + particle_basic_options::IMAGE -> particle_image
    + particle_basic_options::IMAGEMATRIX -> particle_imagematrix

    

    对于parameter类型的options，其具有成员函数confirm，用于在“参数始末”，“参数计算间隔”，“粒子数目”中知二求一。

    三个函数，**confirm_t_begin_end**(),**confirm_para_counting_period**(),**confirm_particle_num**()

    注意，每次通过get，set更改options的属性后都需要调用confirm后内部属性才能对应起来，其不会自动变化。

    

4. 函数**float_scientific_to_fix**

    函数原型：

    **str** **float_scientific_to_fix**(*const* **str** *&* **float_str**, **char_type** **exp_char** = 'e', **char_type** **dot_char** = '.')

    函数功能：

    将浮点数字符串从 科学计数法形式 转换为 定点形式

    参数：

    float_str：科学计数法形式的浮点数字符串

    exp_char：科学计数法形式的浮点字符串用于标志指数的符号，默认为e

    dot_char：科学计数法形式的浮点字符串用于标志小数的符号，默认为.

    函数使用注意事项：

    1. 此函数一般不适用默认的指数字符，我们一般搭配之后的“infer_exponent_char”函数进行使用
    2. 输入的float_str不一定为科学计数法形式，也可以是整数，定点浮点数等等，无论其输入为什么其输出结果一定是定点浮点数。

5. 函数**infer_exponent_char**

    函数原型：

    **char_type** **infer_exponent_char**(*const* **cln**::**cl_print_flags** *&* **flags**, *const* **real_number** *&* **num**)

    函数功能：

    推断一个实数究竟使用什么方式进行科学计数法指数符号

    参数：

    flags：打印标志

    num：实数

    函数使用注意事项：

    函数可能会返回‘\\0’，使用时需要注意判断.



















