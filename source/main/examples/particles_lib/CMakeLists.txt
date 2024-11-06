project(example_particle_lib LANGUAGES CXX)

file(GLOB all_src RELATIVE ${PROJECT_SOURCE_DIR} "*.cpp")

set(target_name example_particle_lib_exe)

add_executable(${target_name} ${all_src})
target_link_libraries(${target_name} PUBLIC base_particle_lib_lib glad_lib)
set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_NAME example_particle_lib)
add_register_target(ROOT ${target_name})
