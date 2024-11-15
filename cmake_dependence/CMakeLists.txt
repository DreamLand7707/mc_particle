cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
project(cmake_dependence LANGUAGES CXX)

set(src ${ROOT_dependency_dir}/${code_name}.cpp)

add_executable(cmake_dependence_executable ${src})

set_target_properties(cmake_dependence_executable
    PROPERTIES
    CXX_STANDARD ${ROOT_dependence_standard}
    RUNTIME_OUTPUT_NAME ${ROOT_dependence_exe_name}
    RUNTIME_OUTPUT_DIRECTORY ${ROOT_dependency_executable_dir}
    SUFFIX ""
    PREFIX ""
)
