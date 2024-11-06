project(OpenGL_GUI LANGUAGES CXX)

file(GLOB all_src RELATIVE ${PROJECT_SOURCE_DIR} "*.cpp")

set(target_name OpenGL_GUI_lib)

add_executable(${target_name} ${all_src})

target_link_libraries(${target_name} PUBLIC glad_lib)
set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_NAME OpenGL_GUI)

add_register_target(ROOT ${target_name})
