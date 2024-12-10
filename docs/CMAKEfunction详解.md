
# FUNCTIONS.CMAKE 详解

---

## 1. `print_list`

```cmake
function(print_list list)
    foreach(list_item IN LISTS ${list})
        message(STATUS ${list_item})
    endforeach()
endfunction()
```

**功能**：遍历并打印给定列表中的每个元素。

**参数**：

- `list`：要打印的列表变量的名称。

**内部语句解析**：

1. `foreach(list_item IN LISTS ${list})`

   - `foreach`：循环指令，用于遍历列表。
   - `list_item`：循环变量，表示当前遍历的列表项。
   - `IN LISTS ${list}`：指定要遍历的列表变量 `${list}`。

2. `message(STATUS ${list_item})`

   - `message`：CMake 命令，用于输出信息。
   - `STATUS`：消息类型，表示一般状态信息。
   - `${list_item}`：当前列表项的值。

**示例**：

```cmake
set(MY_LIST "apple" "banana" "cherry")
print_list(MY_LIST)
```

**输出**：

```
-- apple
-- banana
-- cherry
```

---

## 2. `println`

```cmake
function(println message)
    message(STATUS ${message})
endfunction()
```

**功能**：打印一条状态信息。

**参数**：

- `message`：要打印的消息内容。

**内部语句解析**：

- `message(STATUS ${message})`：直接输出状态信息 `${message}`。

**示例**：

```cmake
println("构建开始")
```

**输出**：

```
-- 构建开始
```

---

## 3. `get_file_directory`

```cmake
function(get_file_directory file_path out_var)
    string(FIND ${file_path} "/" pos REVERSE)

    if(pos EQUAL -1)
        set(temp1 "")
    else()
        string(SUBSTRING ${file_path} 0 ${pos} temp1)
    endif()

    set(${out_var} "${temp1}" PARENT_SCOPE)
endfunction()
```

**功能**：获取文件路径的目录部分，不包含文件名。

**参数**：

- `file_path`：文件的完整路径。
- `out_var`：输出变量的名称，用于存储目录路径。

**内部语句解析**：

1. `string(FIND ${file_path} "/" pos REVERSE)`

   - `string(FIND)`：在字符串中查找子字符串的位置。
   - `${file_path}`：要查找的字符串，即文件路径。
   - `'/'`：要查找的字符，即路径分隔符。
   - `pos`：输出变量，存储找到的索引位置。
   - `REVERSE`：从字符串末尾向前查找，实现获取最后一个 `'/'` 的位置。

2. `if(pos EQUAL -1)`

   - 判断是否找到了 `'/'`。
   - `-1` 表示未找到，即路径中没有 `'/'`，可能是单独的文件名。

3. `set(temp1 "")`

   - 如果未找到 `'/'`，将 `temp1` 设置为空字符串，表示没有目录部分。

4. `else()`

   - 如果找到了 `'/'`，执行以下操作。

5. `string(SUBSTRING ${file_path} 0 ${pos} temp1)`

   - `string(SUBSTRING)`：截取字符串的一部分。
   - 从索引 `0` 开始，截取长度为 `pos` 的子字符串，得到目录路径。
   - 结果存储在 `temp1` 中。

6. `set(${out_var} "${temp1}" PARENT_SCOPE)`

   - 将结果赋值给输出变量 `out_var`。
   - `PARENT_SCOPE`：将变量提升到父作用域，供外部使用。

**示例**：

```cmake
get_file_directory("/home/user/project/main.cpp" DIR)
message("目录路径：${DIR}")
```

**输出**：

```
目录路径：/home/user/project
```

如果输入的 `file_path` 是 `main.cpp`，则 `DIR` 会是空字符串，表示当前目录。

---

## 4. `get_first_line`

```cmake
function(get_first_line str out_var)
    string(FIND ${str} "\n" pos)

    if(pos EQUAL -1)
        set(temp1 ${str})
    else()
        string(SUBSTRING ${str} 0 ${pos} temp1)
    endif()

    set(${out_var} "${temp1}" PARENT_SCOPE)
endfunction()
```

**功能**：获取字符串的第一行内容。

**参数**：

- `str`：输入的字符串，可能包含多行。
- `out_var`：输出变量的名称，用于存储第一行内容。

**内部语句解析**：

1. `string(FIND ${str} "\n" pos)`

   - 在字符串 `str` 中查找换行符 `\n` 的位置。
   - `pos`：输出变量，存储找到的索引位置。

2. `if(pos EQUAL -1)`

   - 判断是否找到了换行符。
   - `-1` 表示未找到，即字符串只有一行。

3. `set(temp1 ${str})`

   - 如果未找到换行符，直接将整个字符串赋值给 `temp1`。

4. `else()`

   - 如果找到了换行符，执行以下操作。

5. `string(SUBSTRING ${str} 0 ${pos} temp1)`

   - 截取从索引 `0` 开始，长度为 `pos` 的子字符串，得到第一行内容。
   - 结果存储在 `temp1` 中。

6. `set(${out_var} "${temp1}" PARENT_SCOPE)`

   - 将结果赋值给输出变量 `out_var`，提升到父作用域。

**示例**：

```cmake
set(MULTILINE "第一行\n第二行\n第三行")
get_first_line("${MULTILINE}" FIRST_LINE)
message("第一行内容：${FIRST_LINE}")
```

**输出**：

```
第一行内容：第一行
```

---

## 5. `open_register_target`

```cmake
function(open_register_target suffix)
    set(__INTERNAL__REGISTER_TRAGETS__${suffix} "" CACHE STRING "targets registered for unified processing")
endfunction()
```

**功能**：初始化一个目标注册列表，用于统一管理一组目标。

**参数**：

- `suffix`：后缀，用于区分不同的注册列表。

**内部语句解析**：

- `set(__INTERNAL__REGISTER_TRAGETS__${suffix} "" CACHE STRING "..." )`

  - 创建一个缓存变量 `__INTERNAL__REGISTER_TRAGETS__${suffix}`，初始值为空列表。
  - `CACHE`：将变量存储到缓存中，供后续使用。
  - `"targets registered for unified processing"`：变量描述信息。

**示例**：

```cmake
open_register_target(ROOT)
```

这将在缓存中创建变量 `__INTERNAL__REGISTER_TRAGETS__ROOT`。

---

## 6. `clear_register_target`

```cmake
function(clear_register_target suffix)
    set(__INTERNAL__REGISTER_TRAGETS__${suffix} "" CACHE STRING "targets registered for unified processing" FORCE)
endfunction()
```

**功能**：清空指定注册列表中的所有目标。

**参数**：

- `suffix`：注册列表的后缀。

**内部语句解析**：

- `set(... FORCE)`

  - 使用 `FORCE`，即使变量已存在，也会强制覆盖。
  - 将注册列表变量重新设置为空列表。

**示例**：

```cmake
clear_register_target(ROOT)
```

---

## 7. `get_register_target`

```cmake
function(get_register_target suffix out_var)
    set(${out_var} "${__INTERNAL__REGISTER_TRAGETS__${suffix}}" PARENT_SCOPE)
endfunction()
```

**功能**：获取指定注册列表中的所有目标。

**参数**：

- `suffix`：注册列表的后缀。
- `out_var`：输出变量的名称，用于存储目标列表。

**内部语句解析**：

- `set(${out_var} "${...}" PARENT_SCOPE)`

  - 将注册列表变量的值赋给输出变量 `out_var`，提升到父作用域。

**示例**：

```cmake
get_register_target(ROOT REGISTERED_TARGETS)
message("注册的目标有：${REGISTERED_TARGETS}")
```

---

## 8. `delete_register_target`

```cmake
function(delete_register_target suffix)
    unset(__INTERNAL__REGISTER_TRAGETS__${suffix} CACHE)
endfunction()
```

**功能**：删除指定的目标注册列表。

**参数**：

- `suffix`：注册列表的后缀。

**内部语句解析**：

- `unset(... CACHE)`

  - 从缓存中删除变量 `__INTERNAL__REGISTER_TRAGETS__${suffix}`。

**示例**：

```cmake
delete_register_target(ROOT)
```

---

## 9. `add_register_target`

```cmake
function(add_register_target suffix target)
    set(_temp1 ${__INTERNAL__REGISTER_TRAGETS__${suffix}})
    list(FIND _temp1 ${target} _temp2)

    if(_temp2 EQUAL -1)
        list(APPEND _temp1 ${target})
        set(__INTERNAL__REGISTER_TRAGETS__${suffix} ${_temp1}
            CACHE STRING "targets registered for unified processing" FORCE)
    endif()

    set_register_target_properties(${target} NOT_DEFAULT_CXX_STANDARD OFF)
    set_register_target_properties(${target} NOT_DEFAULT_OUTPUT_DIRECTORY OFF)
    set_register_target_properties(${target} NOT_DEFAULT_INCLUDE_DIRECTORY OFF)
    set_register_target_properties(${target} NOT_DEFAULT_LIBRARY OFF)
    set_register_target_properties(${target} NOT_DEFAULT_DEFINITION OFF)
    set_register_target_properties(${target} NOT_DEFAULT_DEBUG_SIGN OFF)
    set_register_target_properties(${target} NOT_DEFAULT_COMPILE_OPTIONS OFF)
endfunction()
```

**功能**：将目标添加到指定的注册列表中，并为其设置默认属性。

**参数**：

- `suffix`：注册列表的后缀。
- `target`：要添加的目标名称。

**内部语句解析**：

1. `set(_temp1 ${__INTERNAL__REGISTER_TRAGETS__${suffix}})`

   - 获取当前注册列表的内容，存储在临时变量 `_temp1` 中。

2. `list(FIND _temp1 ${target} _temp2)`

   - 在 `_temp1` 列表中查找是否已存在目标 `target`。
   - `_temp2`：存储找到的索引，`-1` 表示未找到。

3. `if(_temp2 EQUAL -1)`

   - 如果目标未在列表中，执行添加操作。

4. `list(APPEND _temp1 ${target})`

   - 将目标 `target` 添加到 `_temp1` 列表的末尾。

5. `set(... FORCE)`

   - 将更新后的 `_temp1` 列表重新存储到注册列表变量中，使用 `FORCE` 强制覆盖。

6. `set_register_target_properties(${target} ...)`

   - 为目标设置一系列默认属性，初始值为 `OFF`，表示未自定义。

**示例**：

```cmake
add_register_target(ROOT my_executable)
```

---

## 10. `set_register_target_properties`

```cmake
function(set_register_target_properties target ...)
    math(EXPR _temp1 "${ARGC}-1")

    foreach(I RANGE 1 ${_temp1} 2)
        math(EXPR II "${I}+1")
        set(property_name ${ARGV${I}})
        set(property_value ${ARGV${II}})
        set("__${target}__${property_name}" ${property_value}
            CACHE STRING "target: ${target} - key: ${property_name} - value: ${property_value}" FORCE)
    endforeach()
endfunction()
```

**功能**：为目标设置属性键值对。

**参数**：

- `target`：目标名称。
- `...`：属性名称和值的成对参数。

**内部语句解析**：

1. `math(EXPR _temp1 "${ARGC}-1")`

   - 计算参数总数减一，得到要处理的参数数量。

2. `foreach(I RANGE 1 ${_temp1} 2)`

   - `I` 从 `1` 开始，每次递增 `2`，遍历属性名称和值的索引。

3. `math(EXPR II "${I}+1")`

   - 计算属性值的索引 `II`。

4. `set(property_name ${ARGV${I}})`

   - 获取第 `I` 个参数，作为属性名称。

5. `set(property_value ${ARGV${II}})`

   - 获取第 `II` 个参数，作为属性值。

6. `set("__${target}__${property_name}" ${property_value} CACHE STRING "..." FORCE)`

   - 构造缓存变量名 `__${target}__${property_name}`，存储属性值。

**示例**：

```cmake
set_register_target_properties(my_executable OUTPUT_DIR "/bin" CXX_STANDARD 17)
```

这将为目标 `my_executable` 设置两个属性：

- `__my_executable__OUTPUT_DIR`，值为 `"/bin"`。
- `__my_executable__CXX_STANDARD`，值为 `17`。

---

## 11. `get_register_target_property`

```cmake
function(get_register_target_property target property value)
    set(${value} ${__${target}__${property}} PARENT_SCOPE)
endfunction()
```

**功能**：获取目标的指定属性值。

**参数**：

- `target`：目标名称。
- `property`：属性名称。
- `value`：输出变量的名称，用于存储属性值。

**内部语句解析**：

- `set(${value} ${__${target}__${property}} PARENT_SCOPE)`

  - 从缓存变量中获取属性值，赋给输出变量 `value`，提升到父作用域。

**示例**：

```cmake
get_register_target_property(my_executable CXX_STANDARD CXX_STD)
message("C++ 标准：${CXX_STD}")
```

---

## 12. `clear_register_target_property`

```cmake
function(clear_register_target_property target property)
    set(${__${target}__${property}} ""
        CACHE STRING "target: ${target} - key: ${property} - value: " FORCE)
endfunction()
```

**功能**：清除目标的指定属性值，将其设为空字符串。

**参数**：

- `target`：目标名称。
- `property`：属性名称。

**内部语句解析**：

- `set(... FORCE)`

  - 强制将属性值设为空字符串，更新缓存变量。

**示例**：

```cmake
clear_register_target_property(my_executable CXX_STANDARD)
```

---

## 13. `delete_register_target_property`

```cmake
function(delete_register_target_property target property)
    unset(${__${target}__${property}} CACHE)
endfunction()
```

**功能**：删除目标的指定属性变量。

**参数**：

- `target`：目标名称。
- `property`：属性名称。

**内部语句解析**：

- `unset(... CACHE)`

  - 从缓存中删除属性变量。

**示例**：

```cmake
delete_register_target_property(my_executable CXX_STANDARD)
```

---

## 14. `find_proj_flag_file`

```cmake
function(find_proj_flag_file projs_relative_path proj_flag out_proj_flag_files)
    file(GLOB_RECURSE _temp RELATIVE ${projs_relative_path} ${projs_relative_path}/*/${proj_flag})
    set(${out_proj_flag_files} "${_temp}" PARENT_SCOPE)
endfunction()
```

**功能**：在指定路径下递归查找含有特定标记的项目文件。

**参数**：

- `projs_relative_path`：项目源代码的根目录。
- `proj_flag`：项目标记文件的名称（如 `proj`）。
- `out_proj_flag_files`：输出变量的名称，用于存储找到的项目标记文件列表。

**内部语句解析**：

1. `file(GLOB_RECURSE ...)`

   - `GLOB_RECURSE`：递归地搜索文件。
   - `RELATIVE ${projs_relative_path}`：返回相对于该路径的相对路径。

2. `${projs_relative_path}/*/${proj_flag}`

   - 搜索模式，匹配位于任意子目录下名称为 `${proj_flag}` 的文件。

3. `set(${out_proj_flag_files} "${_temp}" PARENT_SCOPE)`

   - 将找到的文件列表赋给输出变量，提升到父作用域。

**示例**：

```cmake
find_proj_flag_file("${ROOT_source_dir}" "${ROOT_project_flag}" ROOT_projs)
```

---

## 15. `make_all_proj_filemap`

```cmake
function(make_all_proj_filemap projs_relative_path proj_flag_files all_projs_file proj_flag out_projs)
    set(proj_content "")
    set(I 0)

    foreach(proj_item IN LISTS proj_flag_files)
        file(READ ${projs_relative_path}/${proj_item} content)
        get_first_line(${content} varb)

        if(${varb} STREQUAL "")
            continue()
        endif()

        if(${I} EQUAL 0)
            file(WRITE ${all_projs_file} ${varb}\n)
        else()
            file(APPEND ${all_projs_file} ${varb}\n)
        endif()

        list(APPEND proj_content ${varb})
        get_file_directory(${proj_item} proj_item_path)
        set(__${proj_flag}_${varb}_path ${projs_relative_path}/${proj_item_path}
            CACHE STRING "project path: ${proj_flag} - ${varb}" FORCE)

        set(I 1)
    endforeach()

    set(${out_projs} "${proj_content}" PARENT_SCOPE)
endfunction()
```

**功能**：生成所有项目的映射文件，并获取项目名称列表。

**参数**：

- `projs_relative_path`：项目源代码的根目录。
- `proj_flag_files`：项目标记文件列表。
- `all_projs_file`：存储所有项目名称的文件路径。
- `proj_flag`：项目标记文件的名称。
- `out_projs`：输出变量，用于存储项目名称列表。

**内部语句解析**：

1. `set(proj_content "")` 和 `set(I 0)`

   - 初始化项目内容列表和计数器。

2. `foreach(proj_item IN LISTS proj_flag_files)`

   - 遍历所有项目标记文件。

3. `file(READ ...)`

   - 读取项目标记文件的内容。

4. `get_first_line(${content} varb)`

   - 获取内容的第一行，作为项目名称。

5. `if(${varb} STREQUAL "")`

   - 如果项目名称为空，跳过当前循环。

6. `if(${I} EQUAL 0)`

   - 判断是否是第一个项目。

7. `file(WRITE ...)` 或 `file(APPEND ...)`

   - 将项目名称写入或追加到 `all_projs_file`。

8. `list(APPEND proj_content ${varb})`

   - 将项目名称添加到 `proj_content` 列表中。

9. `get_file_directory(${proj_item} proj_item_path)`

   - 获取项目标记文件的所在目录。

10. `set(__${proj_flag}_${varb}_path ...)`

    - 存储项目路径到缓存变量，供后续使用。

11. `set(I 1)`

    - 更新计数器。

12. `set(${out_projs} "${proj_content}" PARENT_SCOPE)`

    - 将项目名称列表赋给输出变量。

**示例**：

```cmake
make_all_proj_filemap(
    "${ROOT_source_dir}",
    "${ROOT_projs}",
    "${ROOT_all_projs_file}",
    "${ROOT_project_flag}",
    ROOT_proj_content
)

主要目的是得到如下关系：

proj-test1 ===> __proj_test1_path = ${ROOT_DIR}/source/dir1
proj-test2 ===> __proj_test2_path = ${ROOT_DIR}/source/dir2

```

---

## 16. `make_dependencies_parser`

```cmake
function(make_dependencies_parser makefile_relative_path ccompiler cxxcompiler code_name output_execuable_name)
    execute_process(
        COMMAND make
        -C ${makefile_relative_path}
        C_COMPLER=${ccompiler}
        CXX_COMPLER=${cxxcompiler}
        OUTPUT_EXE=${output_execuable_name}
        CODE_NAME=${code_name}
    )
endfunction()
```

**功能**：调用 `make` 命令，生成依赖关系分析器。

**参数**：

- `makefile_relative_path`：Makefile 的路径。
- `ccompiler`：C 编译器。
- `cxxcompiler`：C++ 编译器。
- `code_name`：代码名称。
- `output_execuable_name`：输出可执行文件的名称。

**内部语句解析**：

- `execute_process(COMMAND ...)`

  - 执行外部命令行。
  - `make -C ${makefile_relative_path} ...`：在指定目录下执行 `make`。

- 传递编译器和输出可执行文件名称作为参数。

**示例**：

```cmake
make_dependencies_parser(
    "${ROOT_dependency_dir}",
    "${CMAKE_C_COMPILER}",
    "${CMAKE_CXX_COMPILER}",
    "dependency_analyse",
    "${ROOT_dependence_exe_name}"
)
```

---

## 17. `parse_dependencies`

```cmake
function(parse_dependencies execuatble_relative_path executable_name dependencies_file custom_dependence_file all_proj_file out_sort_projs)
    execute_process(
        COMMAND ${execuatble_relative_path}/${executable_name} ${dependencies_file} ${custom_dependence_file} ${all_proj_file}
        OUTPUT_VARIABLE sort_projs
    )
    set(${out_sort_projs} "${sort_projs}" PARENT_SCOPE)
endfunction()
```

**功能**：解析项目的依赖关系，生成排序后的项目列表。

**参数**：

- `execuatble_relative_path`：依赖分析器的路径。
- `executable_name`：依赖分析器的可执行文件名称。
- `dependencies_file`：依赖关系文件。
- `custom_dependence_file`：自定义依赖关系文件。
- `all_proj_file`：所有项目名称文件。
- `out_sort_projs`：输出变量，用于存储排序后的项目列表。

**内部语句解析**：

- `execute_process(COMMAND ...)`

  - 执行依赖分析器，传入必要的文件作为参数。
  - `OUTPUT_VARIABLE sort_projs`：将命令的输出结果存储在变量 `sort_projs` 中。

- `set(${out_sort_projs} "${sort_projs}" PARENT_SCOPE)`

  - 将排序后的项目列表赋值给输出变量。

**示例**：

```cmake
parse_dependencies(
    "${ROOT_dependency_executable_dir}",
    "${ROOT_dependence_exe_name}",
    "${ROOT_dependencies_file}",
    "${ROOT_custom_dependencies_file}",
    "${ROOT_all_projs_file}",
    ROOT_sort_projs
)
```

---

## 18. `add_sorts_projects_directories`

```cmake
function(add_sorts_projects_directories proj_flag sort_projs)
    foreach(proj IN LISTS sort_projs)
        add_subdirectory(${__${proj_flag}_${proj}_path})
    endforeach()
endfunction()
```

**功能**：根据排序后的项目列表，依次添加项目的子目录。

**参数**：

- `proj_flag`：项目标记，用于定位项目路径。
- `sort_projs`：排序后的项目名称列表。

**内部语句解析**：

- `foreach(proj IN LISTS sort_projs)`

  - 遍历每个项目名称。

- `add_subdirectory(${__${proj_flag}_${proj}_path})`

  - 添加项目的子目录，路径从缓存变量中获取。

**示例**：

```cmake
add_sorts_projects_directories("${ROOT_project_flag}" "${ROOT_sort_projs}")
```

---

以上就是 

functions.cmake

 建议参考 [CMake 官方文档](https://cmake.org/cmake/help/latest/manual/cmake-commands.7.html) 以获得更深入的理解。

