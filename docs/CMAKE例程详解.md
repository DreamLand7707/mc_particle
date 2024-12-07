
# CMake 例程详解

## CMake 最低版本要求

```cmake
cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
```
指定项目所需的最低 CMake 版本为 3.25，如果不满足则会抛出致命错误。

## 定义项目

```cmake
project(mc_particle LANGUAGES C CXX)
```
定义项目名为 `mc_particle`，并指定使用 C 和 C++ 语言。

## 配置选项

```cmake
set(ROOT_DIR ${PROJECT_SOURCE_DIR})
```
设置根目录路径为项目源目录，也就是根 

`CMakeLists.txt`

 所在的目录。

```cmake
set(ROOT_source_dir ${ROOT_DIR}/source)
set(ROOT_include_dir ${ROOT_DIR}/include)
set(ROOT_dependency_dir ${ROOT_DIR}/cmake_dependence)
set(ROOT_dependency_executable_dir ${ROOT_DIR}/cmake_dependence)
```
设置项目中不同部分的目录路径：

- **ROOT_source_dir**：此 `CMakeLists` 所管理的所有“子工程”的“工程标记文件”搜索相对路径
- **ROOT_include_dir**：用于存储头文件路径
- **ROOT_dependency_dir**：用于存储此 `CMakeLists` 所使用的“依赖分析程序”的源文件路径
- **ROOT_dependency_executable_dir**：用于存储此 `CMakeLists` 所使用的“依赖分析程序”的可执行文件路径

```cmake
set(ROOT_dependencies_file ${ROOT_DIR}/dependence)
set(ROOT_custom_dependencies_file ${ROOT_DIR}/custom_dependence)
set(ROOT_all_projs_file ${ROOT_DIR}/all_projs)
```
设置与项目依赖相关的文件路径：

- **ROOT_dependencies_file**：用于存储此 `CMakeLists` 所使用的依赖关系文件的完整路径（包含路径及文件名）
- **ROOT_all_projs_file**：用于存储此 `CMakeLists` 所使用的存储着所有工程的名字的文件完整路径（包含路径及文件名），此文件以后简称名字文件
- **ROOT_project_flag**：用于存储此 `CMakeLists` 所用于判断子工程所使用的“标记文件”的名字，以后简称工程标记

```cmake
set(ROOT_project_flag "proj")
set(ROOT_dependence_exe_name "dependence_analyse.exe")
```
设置工程标记文件名和依赖分析可执行文件的名称。

## 包含自定义的函数和宏定义

```cmake
include(functions.cmake)
include(macros.cmake)
```
包含自定义的函数和宏定义文件。

```cmake
include(customize.cmake)
```
包含自定义的配置文件。

## 查找工程标记文件

```cmake
find_proj_flag_file(
    "${ROOT_source_dir}"
    "${ROOT_project_flag}"
    ROOT_projs
)
```
查找所有带有特定标记（"proj"）的工程文件，并将结果存储在变量 `ROOT_projs` 中。结果如下：`dir1/proj;dir2/proj`

## 创建工程路径映射

```cmake
make_all_proj_filemap(
    "${ROOT_source_dir}"
    "${ROOT_projs}"
    "${ROOT_all_projs_file}"
    "${ROOT_project_flag}"
    ROOT_proj_content
)
```
创建一个映射文件，包含所有工程的路径信息。主要目的是得到如下关系：

```
proj-test1 ===> __proj_test1_path = ${ROOT_DIR}/source/dir1
proj-test2 ===> __proj_test2_path = ${ROOT_DIR}/source/dir2
```

## 生成依赖分析器

```cmake
make_dependencies_parser(
    "${ROOT_dependency_dir}"
    "${CMAKE_C_COMPILER}"
    "${CMAKE_CXX_COMPILER}"
    "dependency_analyse"
    "${ROOT_dependence_exe_name}"
)
```
生成依赖分析可执行文件。此函数执行后在指定路径生成依赖分析可执行文件。

## 解析项目依赖

```cmake
parse_dependencies(
    "${ROOT_dependency_executable_dir}"
    "${ROOT_dependence_exe_name}"
    "${ROOT_dependencies_file}"
    "${ROOT_custom_dependencies_file}"
    "${ROOT_all_projs_file}"
    ROOT_sort_projs
)
```
解析项目依赖并生成一个排序后的工程列表。这个命令执行一个外部程序，即依赖分析器。依赖关系文件需要自己编写，格式为 `A - B C D` 表示 A 依赖于 B、C 和 D。

## 初始化目标注册表

```cmake
open_register_target(ROOT)
clear_register_target(ROOT)
```
使用 `open_register_target(ROOT)` 创建名为 `__INTERNAL__REGISTER_TRAGETS__ROOT` 的缓存变量，用于存储所有注册的目标。使用 `clear_register_target(ROOT)` 清空注册表内容。注册目标需要在项目中手动添加。

## 查找外部库和软件包

```cmake
find_package(Boost CONFIG REQUIRED COMPONENTS filesystem iostreams locale)
find_package(Eigen3 CONFIG REQUIRED)
find_package(GSL REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(ICU REQUIRED COMPONENTS data i18n io test tu uc)
```
查找所需的外部库和软件包。

- **Boost**：
  - **filesystem**：提供跨平台的文件和目录操作功能。
  - **iostreams**：扩展 C++ 的输入输出流功能，支持更多格式。
  - **locale**：处理国际化和地区特定的格式。
- **Eigen3**：高效的 C++ 线性代数库，支持矩阵和向量运算、几何变换、数值解法等。
- **GSL (GNU Scientific Library)**：提供广泛的数学函数和算法，包括线性代数、随机数生成、优化等。
- **PkgConfig**：用于管理和查询库的编译和链接参数，简化依赖配置。
- **glfw3**：用于创建窗口、处理输入和管理 OpenGL 上下文的跨平台库，常用于图形应用开发。
- **OpenGL**：跨平台的图形 API，用于渲染 2D 和 3D 图形，广泛应用于游戏和图形应用程序。
- **ICU (International Components for Unicode)**：
  - **data**、**i18n**、**io**、**test**、**tu**、**uc** 组件：提供 Unicode 和全球化支持，包括文本处理、日期时间格式化、语言翻译等功能。

```cmake
pkg_check_modules(gmp REQUIRED IMPORTED_TARGET gmp)
pkg_check_modules(gmpxx REQUIRED IMPORTED_TARGET gmpxx)
```
使用 `pkg-config` 查找 `gmp` 和 `gmpxx` 库。

- **gmp**：GNU 多精度算术库，提供高精度的整数、有理数和浮点数运算功能，常用于需要高精度计算的应用。
- **gmpxx**：`gmp` 的 C++ 接口，提供面向对象的封装，使得在 C++ 项目中更方便地使用 `gmp` 的功能。

## 添加工程子目录

```cmake
add_sorts_projects_directories(
    "${ROOT_project_flag}"
    "${ROOT_sort_projs}"
)
```
根据排序后的工程列表添加工程的子目录到 CMake 构建系统。主要目的是执行 `add_subdirectory`。

## 获取注册目标列表

```cmake
get_register_target(ROOT r_targets)
```
获取所有注册的目标列表。注意，注册需要在项目中手动添加。例如，在 

`自己项目的CMakeLists.txt`

 中：

```cmake
project(test1 LANGUAGES CXX)
file(GLOB all_src RELATIVE ${PROJECT_SOURCE_DIR} "*.cpp")
set(target_name test1_exe)
add_executable(${target_name} ${all_src})
target_link_libraries(${target_name} PUBLIC base_particle_lib_lib glad_lib)
set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_NAME test1)
add_register_target(ROOT ${target_name})
```

另外，`add_register_target(ROOT ${target_name})` 包含以下步骤：

```cmake
set_register_target_properties(${target} NOT_DEFAULT_CXX_STANDARD OFF)
set_register_target_properties(${target} NOT_DEFAULT_OUTPUT_DIRECTORY OFF)
set_register_target_properties(${target} NOT_DEFAULT_INCLUDE_DIRECTORY OFF)
set_register_target_properties(${target} NOT_DEFAULT_LIBRARY OFF)
set_register_target_properties(${target} NOT_DEFAULT_DEFINITION OFF)
set_register_target_properties(${target} NOT_DEFAULT_DEBUG_SIGN OFF)
set_register_target_properties(${target} NOT_DEFAULT_COMPILE_OPTIONS OFF)
```

## 启用测试

```cmake
enable_testing()
```
启用测试功能。

## 遍历所有注册的目标，设置其属性

```cmake
foreach(target IN LISTS r_targets)
    # 遍历所有注册的目标，设置其属性：

    get_register_target_property(${target} NOT_DEFAULT_CXX_STANDARD temp)
    if(NOT ${temp})
        set_target_properties(${target} PROPERTIES
            C_STANDARD 23
            CXX_STANDARD 26
        )
    endif()
    # 设置 C 和 C++ 标准。

    get_register_target_property(${target} NOT_DEFAULT_OUTPUT_DIRECTORY temp)
    if(NOT ${temp})
        set_target_properties(${target} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${ROOT_DIR}/Debug/${target}"
            ARCHIVE_OUTPUT_DIRECTORY "${ROOT_DIR}/Lib/${target}"
        )
    endif()
    # 设置目标的输出目录。

    get_register_target_property(${target} NOT_DEFAULT_INCLUDE_DIRECTORY temp)
    if(NOT ${temp})
        target_include_directories(${target}
            PUBLIC ${ROOT_include_dir}
        )
    endif()
    # 设置目标的包含目录。

    get_register_target_property(${target} NOT_DEFAULT_LIBRARY temp)
    if(NOT ${temp})
        target_link_libraries(${target} PUBLIC
            Eigen3::Eigen
            Boost::filesystem Boost::iostreams Boost::locale
            GSL::gsl GSL::gslcblas
            ginac
            cln
            PkgConfig::gmp PkgConfig::gmpxx
            glfw
            OpenGL::GL OpenGL::GLU
            icudt.dll icuin.dll icuio.dll icutest.dll icutu.dll icuuc.dll
        )
    endif()
    # 链接目标所需的库。

    get_register_target_property(${target} NOT_DEFAULT_DEFINITION temp)
    if(NOT ${temp})
        target_compile_definitions(${target}
            PUBLIC UNICODE
            PUBLIC _UNICODE
            PUBLIC LOCAL=1
            PUBLIC "ROOT_DIR=\"${ROOT_DIR}\""
        )
    endif()
    # 设置目标的编译定义。

    get_register_target_property(${target} NOT_DEFAULT_DEBUG_SIGN temp)
    if(NOT ${temp})
        if(${CMAKE_BUILD_TYPE} EQUAL Debug)
            target_compile_options(${target}
                PUBLIC "-g"
            )
        endif()
    endif()
    # 添加调试符号。

    get_register_target_property(${target} NOT_DEFAULT_COMPILE_OPTIONS temp)
    if(NOT ${temp})
        target_compile_options(${target}
            PUBLIC "--target=x86_64-pc-windows-gnu"
            PUBLIC -Wno-deprecated-enum-compare-conditional
        )
    endif()
    # 设置目标的编译选项。
endforeach()
```

### 具体操作解释

1. **设置 C 和 C++ 标准**

    ```cmake
    get_register_target_property(${target} NOT_DEFAULT_CXX_STANDARD temp)
    if(NOT ${temp})
        set_target_properties(${target} PROPERTIES
            C_STANDARD 23
            CXX_STANDARD 26
        )
    endif()
    ```
    - **作用**：如果目标未设置 `NOT_DEFAULT_CXX_STANDARD`，则为其设置 C 标准为 C23 和 C++ 标准为 C++26。

2. **设置目标的输出目录**

    ```cmake
    get_register_target_property(${target} NOT_DEFAULT_OUTPUT_DIRECTORY temp)
    if(NOT ${temp})
        set_target_properties(${target} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${ROOT_DIR}/Debug/${target}"
            ARCHIVE_OUTPUT_DIRECTORY "${ROOT_DIR}/Lib/${target}"
        )
    endif()
    ```
    - **作用**：如果目标未设置 `NOT_DEFAULT_OUTPUT_DIRECTORY`，则为其设置可执行文件输出到 `${ROOT_DIR}/Debug/${target}`，库文件输出到 `${ROOT_DIR}/Lib/${target}`。

3. **设置目标的包含目录**

    ```cmake
    get_register_target_property(${target} NOT_DEFAULT_INCLUDE_DIRECTORY temp)
    if(NOT ${temp})
        target_include_directories(${target}
            PUBLIC ${ROOT_include_dir}
        )
    endif()
    ```
    - **作用**：如果目标未设置 `NOT_DEFAULT_INCLUDE_DIRECTORY`，则为其添加包含目录 `${ROOT_include_dir}`。

4. **链接目标所需的库**

    ```cmake
    get_register_target_property(${target} NOT_DEFAULT_LIBRARY temp)
    if(NOT ${temp})
        target_link_libraries(${target} PUBLIC
            Eigen3::Eigen
            Boost::filesystem Boost::iostreams Boost::locale
            GSL::gsl GSL::gslcblas
            ginac
            cln
            PkgConfig::gmp PkgConfig::gmpxx
            glfw
            OpenGL::GL OpenGL::GLU
            icudt.dll icuin.dll icuio.dll icutest.dll icutu.dll icuuc.dll
        )
    endif()
    ```
    - **作用**：如果目标未设置 `NOT_DEFAULT_LIBRARY`，则为其链接所需的库，包括 Eigen3、Boost、GSL、ginac、cln、PkgConfig、glfw、OpenGL 和 ICU。

5. **设置目标的编译定义**

    ```cmake
    get_register_target_property(${target} NOT_DEFAULT_DEFINITION temp)
    if(NOT ${temp})
        target_compile_definitions(${target}
            PUBLIC UNICODE
            PUBLIC _UNICODE
            PUBLIC LOCAL=1
            PUBLIC "ROOT_DIR=\"${ROOT_DIR}\""
        )
    endif()
    ```
    - **作用**：如果目标未设置 `NOT_DEFAULT_DEFINITION`，则为其添加编译定义 `UNICODE`、`_UNICODE`、`LOCAL=1` 以及 `ROOT_DIR` 的路径。

6. **添加调试符号**

    ```cmake
    get_register_target_property(${target} NOT_DEFAULT_DEBUG_SIGN temp)
    if(NOT ${temp})
        if(${CMAKE_BUILD_TYPE} EQUAL Debug)
            target_compile_options(${target}
                PUBLIC "-g"
            )
        endif()
    endif()
    ```
    - **作用**：如果目标未设置 `NOT_DEFAULT_DEBUG_SIGN`，且当前构建类型为`Debug`，则为其添加编译选项 `-g` 以生成调试信息。

7. **设置目标的编译选项**

    ```cmake
    get_register_target_property(${target} NOT_DEFAULT_COMPILE_OPTIONS temp)
    if(NOT ${temp})
        target_compile_options(${target}
            PUBLIC "--target=x86_64-pc-windows-gnu"
            PUBLIC -Wno-deprecated-enum-compare-conditional
        )
    endif()
    ```
    - **作用**：如果目标未设置 `NOT_DEFAULT_COMPILE_OPTIONS`，则为其添加编译选项 `--target=x86_64-pc-windows-gnu`（用于指定目标平台）和 `-Wno-deprecated-enum-compare-conditional`（用于禁用特定编译器警告）。

    


