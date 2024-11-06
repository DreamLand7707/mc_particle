project(glad_lib LANGUAGES CXX)

file(GLOB all_src RELATIVE ${PROJECT_SOURCE_DIR} "glad.c")

set(target_name glad_lib)

add_library(${target_name} STATIC ${all_src})

set_target_properties(${target_name} PROPERTIES LIBRARY_OUTPUT_NAME glad_lib)

add_register_target(ROOT ${target_name})
