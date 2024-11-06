## 这是一个用于自动化生成我的世界粒子的工程

本工程使用C++

/include 头文件，后缀使用hpp

/source 源文件，后缀使用cpp

/source/main 工程的主要源文件，可以在source下添加自己的测试代码，但是不要合并到main分支

/test 测试文件，后缀使用cpp

/resource 资源文件，这个文件夹用于存放资源文件等，资源文件的使用是分支间共享的，会有较大概率出现合并冲突，需要谨慎使用

/Color Block 放置指令教程文件

/docs 本工程文档



文件格式必须使用 CRLF换行 UTF-8编码



## GiNaC和CLN安装

GiNaC基于CLN，需要先安装CLN

安装CLN之前，请先在clang64终端中输入 `pacman -S mingw-w64-clang-x86_64-gmp` 安装gmp库

cln源码下载网址`https://www.ginac.de/CLN/`

解压后，进入源代码根目录，打开msys2-clang64终端，输入指令，注意，美元符号‘$’不用输入

```sh
$ cd 'directory/to/folder'
$ ./configure
$ make
$ make check
$ make install
```

之后安装GiNaC

源码下载网址`https://www.ginac.de/Download.html`

解压后，进入源代码根目录，打开msys2-clang64终端，输入指令，注意，美元符号‘$’仍然不用输入

```sh
$ cd 'directory/to/folder'
$ export CXXFLAGS="-Wall -O2"
$ ./configure
$ make
$ make check
$ make install
```

之后在CMakeLists中直接对一个目标使用

`target_link_libraries(target PRIVATE ginac cln)`即可实现库的链接

注意注意，ginac必须在cln之前！！



## CLN库教程查询方式

CLN是一个C++的数据类型库，它提供了高精度浮点，大整数等特性。

CLN教程：

在安装完cln库的时候输入

```sh
make html
```

就会自动构建cln库的教程，位置位于doc/cln.html

本教程的数据类型主要使用CLN库提供的数据类型，例如：cl_I(整数),cl_R(实数)等



## CLANG-FORMAT 安装

使用clang64-msys2终端指令

`where clang-format`查看是否安装了clang-format，如果显示了路径则安装成功，若没有找到文件则需要使用

`pacman -Ss mingw-w64-clang-x86_64-llvm`来安装

