project(pybind_example LANGUAGES CXX)
set(src pybind_main.cpp)

add_executable(pybind_example ${src})
set_target_properties(
    pybind_example PROPERTIES
    RUNTIME_OUTPUT_NAME pybind_example
    C_STANDARD 20
    CXX_STANDARD 23
)

add_register_target(ROOT pybind_example)
set_register_target_properties(pybind_example NOT_DEFAULT_CXX_STANDARD ON)
set_register_target_properties(pybind_example TARGET_NEED_DETACHED_DIR ON)