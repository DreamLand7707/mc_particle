## CMAKE 工程框架用法，原理说明

教程目录结构：

```dir tree
/（根目录）
|-CMakeLists.txt（根目录CMake）.......................[需自行编写，存在模板]
|-functions.txt （框架函数）..........................[无需自行编写，可修改但需保证功能准确]
|-source
	|-dir1（文件夹）
		|-CMakeLists.txt（子目录1CMake）...................[需自行编写]
		|-proj（子目录1标记文件）............................[需自行编写]
		|-main.cpp（源文件）
	|-dir2（文件夹）
		|-CMakeLists.txt（子目录2CMake）....................[需自行编写]
		|-proj（子目录2标记文件）............................[需自行编写]
		|-main.cpp（源文件）
|-include
	|-test.hpp（头文件）
|-cmake_dependence（文件夹）
	|-dependency_analyse.cpp（依赖分析的程序源文件）......[无需自行编写，可修改但需保证功能准确]
	|-Makefile（make语言脚本）..........................[无需自行编写，可修改但需保证功能准确]
	|-dependence_analyse.exe（编译好的程序文件）..........[自动生成]
	|-...（若干中间文件）................................[自动生成]
|-all_projs（文件，存储着所有的工程的名字）.................[自动生成]
|-dependence（文件，存储着工程之间的依赖关系）..............[需自行编写]
```

根目录模板文件分析：

1~2 line

```cmake
cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
project(mc_particle)
```

定义了根目录下的工程名字和 CMAKE 最小版本号需求，这是老生常谈了

5~16 line

```cmake
set(ROOT_DIR ${PROJECT_SOURCE_DIR})

set(ROOT_source_dir ${ROOT_DIR}/source)
set(ROOT_include_dir ${ROOT_DIR}/include)
set(ROOT_dependency_dir ${ROOT_DIR}/cmake_dependence)
set(ROOT_dependency_executable_dir ${ROOT_DIR}/cmake_dependence)

set(ROOT_dependencies_file ${ROOT_DIR}/dependence)
set(ROOT_all_projs_file ${ROOT_DIR}/all_projs)

set(ROOT_project_flag "proj")
set(ROOT_dependence_exe_name "dependence_analyse.exe")
```

定义关键变量，变量意义说明：

ROOT_DIR，此 cmake 文件所在目录，也就是根 CMakeLists.txt 所在的目录

ROOT_source_dir，此 CMakeLists 所管理的所有“子工程”的“工程标记文件”搜索相对路径

ROOT_include_dir，这不是必须项，只是用于存储头文件路径

ROOT_dependency_dir，用于存储此 CMakeLists 所使用的“依赖分析程序”的源文件的路径

ROOT_dependency_executable_dir，用于存储此 CMakeLists 所使用的“依赖分析程序”的可执行文件路径

ROOT_dependencies_file，用于存储此 CMakeLists 所使用的依赖关系文件的完整路径（包含路径及文件名）

ROOT_all_projs_file，用于存储此 CMakeLists 所使用的存储着所有工程的名字的文件完整路径（包含路径及文件名），此文件以后简称名字文件

ROOT_project_flag，用于存储此 CMakeLists 所用于判断子工程所使用的“标记文件”的名字，以后简称工程标记

ROOT_dependence_exe_name，用于存储此 CMakeLists 所使用的“依赖分析程序”的可执行文件的具体名字

此框架整体思路：

​ 考虑到我们可能需要在工程中构建多种的可执行文件，多种的库来满足不同的要求。这些可执行文件和库我们称之为目标。

{概念解释–“添加文件到目标”-指的是一个目标，比如说可执行文件，它的生成需要使用很多.cpp 源文件，这些源文件会编译成这个可执行文件，而我们指定哪些.cpp 文件生成这个可执行文件的过程就是添加这些.cpp 文件到这个目标(可执行文件)}

​ 假如说我们把每个工程的源文件和头文件都放到一个文件夹里，并且在 Cmake 文件中逐个添加它们到每个目标，这是一个非常麻烦的事情，因为当一个目标的文件多至几十个时候，逐一添加文件将是费事又费力的工作，而一旦又添加新的文件，混乱的文件结构又很容易出错。

​ 所以为了避免复杂的文件结构，我初步打算采取通过文件夹的方式来分类不同的工程，也就是说一个文件夹内就是一个工程，这是一个整体。这是一个可行的思路，但是有一个问题，我们可以说一个工程是一个文件夹，但并不是所有的文件夹都是可以被看作是工程的。所以我们需要标记哪些文件夹可以被视为工程，哪些不可以被视为工程，这就是标记文件的作用。只要这个文件夹内出现了标记文件，那么这个文件夹就会被认为是一个独立的工程。

​ 每一个工程需要有一个独一无二的名字，这个名字需要程序员自己起名字，而这个名字被存储于工程标记文件的第一行且不可以为空。

​ 每个工程都有属于自己的特征，例如 A 工程使用了 Eigen 库而 B 工程并没使用，但是 C 工程除了 Eigen 库还额外使用的 cln 库，这些都是每个工程自己的特点，为了便于管理每个工程自己的特点，我们在每个工程的标记文件的同名目录下再添加一个额外的 cmakelists.txt 用于管理每个独立工程自己的特点。实际上，每个独立工程所做的最基本的事情也正是靠着这个 cmake 文件来实现的。标记文件只用于标记工程，但真做事还是得依靠这个 cmake 文件。

​ cmake 提供了一个函数用于对一个额外的目录执行 cmake，这个命令叫做 add_subdirectory()，它接受一个路径作为参数，这个路径下的 cmakelists.txt 将被 cmake 执行，从而在整个构建过程中引入子文件夹中的 cmakelists.txt 的目标。

​ 工程之间彼此可以有联系，比如说 A 工程生成了一个库，而这个库被 B 工程所使用，这就是独立工程之间的联系，这种联系被称为工程之间的依赖关系。在 cmake 中，在这种情况下，如果 B 工程比 A 工程率先被引入的话，就会出错，报错信息是 B 未发现有 A 出现，所以我们需要保证每个目标在被引入前，它所依赖的目标必须全部被引入，而它所依赖的目标也会有依赖的目标，这样递归下去，就形成了目标的依赖链。

​ （注意，这里混用了工程和目标的概念，为什么会这样做呢，因为我们对于一个目标来说，它只会依赖于另一个目标，比如说可执行文件依赖于库，库依赖于库，而这样的目标又是包含于一个工程中的，所以从表面上来看就是工程在依赖于工程，但实际上我们是一个工程的目标依赖于另一个工程的目标，但是我们控制的并不是目标，我们控制的是工程）

​ 实际上，这样的依赖关系链的分析对于 cmake 脚本来说分析起来是相当困难的，所以我们可以编写一个更适合处理这样的情况的程序，通过给它输入文件来进行分析，而分析结果直接输出给 cmake 即可。这里我将使用 cpp 编写这个程序。

​ cmake 提供了一个函数**execute_process**来执行系统指令，类似于 c 语言中的 system，而执行程序的指令正是系统指令，所以依赖分析程序的执行就可以通过这个函数来启动执行。当然，它也提供了很多参数，用于运行使用，我们将会在之后讲到。

​ 当我们把合适的工程顺序分析完后，我们将会把正确的添加工程的顺序返回给 cmake，cmake 就会按照顺序逐一对工程所在文件夹使用 add_subdirectory 来用正确的顺序引入目标。

​ 这就是此 cmake 构建系统所需要解决的问题以及解决问题的思路。

此框架包含的函数：

-   function(**find_proj_flag_file** projs_relative_path proj_flag out_proj_flag_files)

    projs_relative_path 其意义等同于上面的 ROOT_source_dir 变量

    proj_flag 其意义等同于上面的 ROOT_project_flag 变量

    out_proj_flag_files 函数输出，是一个列表，每一项是标记文件相对于 projs_relative_path 的完整相对路径

    在本例中输出变量的值将是

    `dir1/proj;dir2/proj`

    使用分号分隔列表的不同项

-   function(**make_all_proj_filemap** projs_relative_path proj_flag_files all_projs_file proj_flag out_projs)

    projs_relative_path 其意义等同于上面的 ROOT_source_dir 变量

    proj_flag_files 其意义等同于上面获得的 out_proj_flag_files 列表

    all_projs_file 其意义等同于上面的 ROOT_all_projs_file 变量

    proj_flag 其意义等同于上面的 ROOT_project_flag 变量

    out_projs 获取输出，这是个列表，包含了所有的工程名字，一般用不上

    这个函数做了两个事情，

    第一，它将所有的工程的名字输出到了 all_projs_file 这个变量存储的文件名字的这个文件里面，

    第二，它建立了一个从“proj_flag”-“工程名字”到“工程所在的完整目录”的映射关系

    假设我们子目录 1 标记文件中的内容是“test1”，子目录 2 标记文件中的内容是“test2”，那我们的 all_projs 文件的内容将是

    ```
    test1
    test2
    ```

    我们建立的映射关系将是

    `proj-test1 ===> __proj_test1_path = ${ROOT_DIR}/source/dir1`

    `proj-test2 ===> __proj_test2_path = ${ROOT_DIR}/source/dir2`

    实际上 cmake 并没有映射，我们是通过定义变量的名字来实现映射的，cmake 里面的变量的名字是可以使用变量的值的，例如

    ```cmake
    set(a "ouch")
    set(a_${a} "ippp")
    ```

    在这里我们定义了两个变量，一个叫做 a，一个叫做 a_ouch

    所以正如我们所见，cmake 没有整数没有浮点数，所有的变量本质上来说都是字符串。

-   function(**make_dependencies_parser** makefile_relative_path ccompiler cxxcompiler code_name output_execuable_name)

    这个函数做了一个事情，就是将依赖分析程序的源代码编译成可执行文件

    makefile_relative_path 含义类似于变量 ROOT_dependencies_file，实际上指的是 Makefile 所在的路径

    ccompiler cxxcompiler c 编译器和 cpp 编译器的名字，推荐使用 cmake 自带变量\${CMAKE_C_COMPILER} \${CMAKE_CXX_COMPILER}

    code_name 依赖程序源文件除去.cpp 后缀剩余的那部分

    output_execuable_name 含义类似于变量 ROOT_dependence_exe_name，但也可以不是

    此函数执行后在 makefile_relative_path 下（如果仅在 output_execuable_name 内指定了可执行文件的名字）或某处（output_execuable_name 内指定的路径）生成依赖分析可执行文件

-   function(**parse_dependencies** execuatble_relative_path executable_name dependencies_file all_proj_file out_sort_projs)

    这个函数用于执行依赖分析可执行文件

    execuatble_relative_path 可执行程序文件所在路径

    executable_name 含义类似于变量 ROOT_dependence_exe_name

    dependencies_file 含义类似于 ROOT_dependencies_file

    all_proj_file 含义类似于 ROOT_all_projs_file

    out_sort_projs 输出变量，是排序好的工程列表，工程应从列表的首项到末项逐个添加进来

-   function(**add_sorts_projects_directories** proj_flag sort_projs)

    这个函数通过排序好的工程列表和 proj_flag 来逐个添加工程

    这里，你可能会有疑问，add_subdirectory 是用于 cmake 添加子文件夹的，那么传入的参数并没有子文件夹，是如何找到子文件夹的呢？

    这里，我们就用到了在 make_all_proj_filemap 函数中建立的映射了，我们已经知晓了 proj_flag 也知道了工程名字，根据我们建立的映射，我们就可以知道不同工程所对应的文件夹了。
