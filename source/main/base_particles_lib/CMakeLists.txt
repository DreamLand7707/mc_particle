project(base_particles_lib LANGUAGES CXX)

file(GLOB all_src RELATIVE ${PROJECT_SOURCE_DIR} "*.cpp")

set(target_name base_particle_lib_lib)

add_library(${target_name} STATIC ${all_src})

set_target_properties(${target_name} PROPERTIES LIBRARY_OUTPUT_NAME mc_particle)

add_register_target(ROOT ${target_name})
