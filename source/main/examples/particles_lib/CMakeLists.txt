project(example_particle_lib LANGUAGES CXX)

add_executable(particle_lib_example1 main.cpp)
add_executable(particle_lib_example2 main2.cpp)

target_link_libraries(particle_lib_example1 PUBLIC base_particle_lib glad_lib svg_analyse)
target_link_libraries(particle_lib_example2 PUBLIC base_particle_lib glad_lib svg_analyse)

add_register_target(ROOT particle_lib_example1)
add_register_target(ROOT particle_lib_example2)
