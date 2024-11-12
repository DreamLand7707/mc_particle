## particle_basic_options __类__

### 参数说明

particle_name (粒子名): "minecraft:end_rod"
coord (坐标): {0, 0, 0}
initial_velocity (初始速度): {0, 0, 0}
velt_counting_period (速度计算间隔): 1.0
age (粒子寿命): 0
relative_coord (相对坐标): false

## particle_normal_options __类__

### 参数说明

rgba: {1.0, 1.0, 1.0, 1.0} (颜色与透明度)
range: {1.0, 1.0, 1.0} (范围)
count: 100 (粒子数)
velocity_expr: 速度表达式
length 和 prec: 按默认值即可

### 指令示例

/particleex normal <颗粒> <坐标> <颜色> <速度> <范围> <数量> [寿命] [速度表达式] [计算间隔] [组]

### 参考代码

`particle_basic_options basic("minecraft:end_rod", {0, 0, 0}, {1, 1, 1}, 1.0, 1000, true); particle_normal_options normal({0.9, 0.4, 0.2, 0.9}, {1.0, 1.0, 1.0}, 1000, basic); str velocity_expr = "(vx,,vy,,vz)=(0,0,-1,,0,0,0,,1,0,0)*(x,,y,,z)"; str result = normal.format(velocity_expr, 0, 6);`

### 结果示例

`particleex normal minecraft:end_rod ~0.000000 ~0.000000 ~0.000000 0.900000 0.400000 0.200000 0.900000 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000 1000 1000 "(vx,,vy,,vz)=(0,0,-1,,0,0,0,,1,0,0)*(x,,y,,z)" 1.000000`

## particle_conditional_options __类__

### 参数说明

rgba: {1.0, 1.0, 1.0, 1.0} (颜色与透明度)
range: {1.0, 1.0, 1.0} (范围)
counting_period: 0.1 (计数间隔)
注意：conditional_expr 表示对范围的约束，而不是轨迹表达式。

### 指令示例

/particleex conditional <颗粒> <坐标> <颜色> <速度> <范围> <条件表达式> [计算间隔] [寿命] [速度表达式] [计算间隔] [组]

## parameter_particle_options __类__

### 参数说明

t_begin_end: {0.0, 10.0}
particle_num: 100
para_counting_period: 0.1
polar: false

## particle_parameter_options __类__

### 参数说明

rgba: {1.0, 1.0, 1.0, 1.0}
parameter_expr: 规则表达式，例如 "x,y=t,sin(t)" 或 "x=t,y=sin(t)" 表示轨迹
t_begin_end: 指参数方程中 t 的范围
para_counting_period: 指参数方程中 t 的刻度

### 指令示例

/particleex parameter <颗粒> <坐标> <颜色> <速度> <begin> <end> <表达式> [计算间隔] [寿命] [速度表达式] [计算间隔] [组]

### 参考代码

`particle_basic_options basic("minecraft:end_rod", {0, 0, 0}, {1, 1, 1}, 1.0, 1000, true); parameter_particle_options para_part({0.0, 10.0}, 100, 0.1, false, basic); para_part.set_t_begin_end(para_part.confirm_t_begin_end()); particle_parameter_options part_para({1.0, 1.0, 1.0, 1.0}, para_part); str parameter_expr = "x,y,z=t,sin(t),t"; str velocity_expr = "(vx,,vy,,vz)=(1,0,0,,0,1,0,,0,0,1)*(x,,y,,z))"; str result = part_para.format(parameter_expr, velocity_expr, 0, 6);`

### 结果示例

`particleex parameter minecraft:end_rod ~0.000000 ~0.000000 ~0.000000 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000 0.000000 9.950000 "x,y,z=t,sin(t),t" 0.100000 1000 "(vx,,vy,,vz)=(1,0,0,,0,1,0,,0,0,1)*(x,,y,,z))" 1.000000`

## tick_parameter_particle_options __类__

### 参数说明

gent_counting_per_tick_: 每 tick 计算次数，较大的 CPT 值会加速粒子生成，用于更平滑的动画效果

### 指令示例

particleex tickparameter <颗粒名> <x> <y> <z> <红> <绿> <蓝> <透明度> <亮度> <vx> <vy> <vz> <tStart> <tEnd> <表达式> [计算间隔] [CPT] [寿命] [速度表达式] [计算间隔] [组] [参数]

### 参考代码

`particle_basic_options basic("minecraft:end_rod", {0, 0, 0}, {1, 1, 1}, 1.0, 1000, true); parameter_particle_options para_part({0.0, 100.0}, 100, 0.1, false, basic); para_part.set_t_begin_end(para_part.confirm_t_begin_end()); tick_parameter_particle_options tick_para(100, para_part); particle_tick_parameter_options part_tick({0.9, 0.2, 0.7, 1.0}, tick_para); str parameter_expr = "x,y,z=t,sin(t),t"; str velocity_expr = "(vx,,vy,,vz)=(1,0,0,,0,1,0,,0,0,1)*(x,,y,,z))"; str result = part_tick.format(parameter_expr, velocity_expr, 0, 6);`

### 结果示例

`particleex tickparameter minecraft:end_rod ~0.000000 ~0.000000 ~0.000000 0.900000 0.200000 0.700000 1.000000 1.000000 1.000000 1.000000 0.000000 99.50000 "x,y,z=t,sin(t),t" 0.100000 1 100 "(vx,,vy,,vz)=(1,0,0,,0,1,0,,0,0,1)*(x,,y,,z))" 1.000000`

#### 对于rgb相关的指令没有涉及，可以到隔壁的linear指南里参考。

#### 对于image相关的指令没有涉及，暂时后续程序并未用到。