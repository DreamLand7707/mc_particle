project(svg_analyse LANGUAGES CXX)

file(GLOB all_src RELATIVE ${PROJECT_SOURCE_DIR} "*.cpp")
set(target_name svg_analyse)
add_library(${target_name} STATIC ${all_src})
set_target_properties(${target_name} PROPERTIES OUTPUT_NAME svg_analyse)

add_register_target(ROOT ${target_name})
