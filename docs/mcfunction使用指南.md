运行mcfunction需要遵循以下步骤（此处以项目内例程为例）


1.将游戏内任意存档的datapacks复制进剪贴板 如****\MC\.minecraft\saves\test12\datapacks
2.进入source\main\examples\particles_lib\main.cpp
3.将剪贴板内路径代替第九行dp内的第一个路径
4.进入游戏
5.在游戏内选取两个点的三维坐标，代替34行的points[0]和points[2]
6.在游戏内输入指令give @s command_block获得命令方块
7.在命令方块内输入指令function example:test_func
8.启动命令方块
