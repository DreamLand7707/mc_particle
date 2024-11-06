cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
project(mc_particle LANGUAGES C CXX)

# Options
set(ROOT_DIR ${PROJECT_SOURCE_DIR})

set(ROOT_source_dir ${ROOT_DIR}/source)
set(ROOT_include_dir ${ROOT_DIR}/include)
set(ROOT_dependency_dir ${ROOT_DIR}/cmake_dependence)
set(ROOT_dependency_executable_dir ${ROOT_DIR}/cmake_dependence)

set(ROOT_dependencies_file ${ROOT_DIR}/dependence)
set(ROOT_custom_dependencies_file ${ROOT_DIR}/custom_dependence)
set(ROOT_all_projs_file ${ROOT_DIR}/all_projs)

set(ROOT_project_flag "proj")
set(ROOT_dependence_exe_name "dependence_analyse.exe")

# #########
include(functions.cmake)
include(macros.cmake)

include(customize.cmake)

find_proj_flag_file(
    "${ROOT_source_dir}"
    "${ROOT_project_flag}"
    ROOT_projs
)
make_all_proj_filemap(
    "${ROOT_source_dir}"
    "${ROOT_projs}"
    "${ROOT_all_projs_file}"
    "${ROOT_project_flag}"
    ROOT_proj_content
)

make_dependencies_parser(
    "${ROOT_dependency_dir}"
    "${CMAKE_C_COMPILER}"
    "${CMAKE_CXX_COMPILER}"
    "dependency_analyse"
    "${ROOT_dependence_exe_name}"
)

parse_dependencies(
    "${ROOT_dependency_executable_dir}"
    "${ROOT_dependence_exe_name}"
    "${ROOT_dependencies_file}"
    "${ROOT_custom_dependencies_file}"
    "${ROOT_all_projs_file}"
    ROOT_sort_projs
)

open_register_target(ROOT)
clear_register_target(ROOT)

find_package(Boost CONFIG REQUIRED COMPONENTS filesystem iostreams locale)
find_package(Eigen3 CONFIG REQUIRED)
find_package(GSL REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(ICU REQUIRED COMPONENTS data i18n io test tu uc)

pkg_check_modules(gmp REQUIRED IMPORTED_TARGET gmp)
pkg_check_modules(gmpxx REQUIRED IMPORTED_TARGET gmpxx)

add_sorts_projects_directories(
    "${ROOT_project_flag}"
    "${ROOT_sort_projs}"
)

get_register_target(ROOT r_targets)

enable_testing()

foreach(target IN LISTS r_targets)
    get_register_target_property(${target} NOT_DEFAULT_CXX_STANDARD temp)

    if(NOT ${temp})
        set_target_properties(${target} PROPERTIES
            C_STANDARD 23
            CXX_STANDARD 26
        )
    endif()

    get_register_target_property(${target} NOT_DEFAULT_OUTPUT_DIRECTORY temp)

    if(NOT ${temp})
        set_target_properties(${target} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${ROOT_DIR}/Debug/${target}"
            ARCHIVE_OUTPUT_DIRECTORY "${ROOT_DIR}/Lib/${target}"
        )
    endif()

    get_register_target_property(${target} NOT_DEFAULT_INCLUDE_DIRECTORY temp)

    if(NOT ${temp})
        target_include_directories(${target}
            PUBLIC ${ROOT_include_dir}
        )
    endif()

    get_register_target_property(${target} NOT_DEFAULT_LIBRARY temp)

    if(NOT ${temp})
        target_link_libraries(${target} PUBLIC
            Eigen3::Eigen
            Boost::filesystem Boost::iostreams Boost::locale
            GSL::gsl GSL::gslcblas

            ginac
            cln
            PkgConfig::gmp PkgConfig::gmpxx

            glfw

            OpenGL::GL OpenGL::GLU

            icudt.dll icuin.dll icuio.dll icutest.dll icutu.dll icuuc.dll
        )
    endif()

    get_register_target_property(${target} NOT_DEFAULT_DEFINITION temp)

    if(NOT ${temp})
        target_compile_definitions(${target}
            PUBLIC UNICODE
            PUBLIC _UNICODE
            PUBLIC LOCAL=1
            PUBLIC "ROOT_DIR=\"${ROOT_DIR}\""
        )
    endif()

    get_register_target_property(${target} NOT_DEFAULT_DEBUG_SIGN temp)

    if(NOT ${temp})
        if(${CMAKE_BUILD_TYPE} EQUAL Debug)
            target_compile_options(${target}
                PUBLIC "-g"
            )
        endif()
    endif()

    get_register_target_property(${target} NOT_DEFAULT_COMPILE_OPTIONS temp)

    if(NOT ${temp})
        target_compile_options(${target}
            PUBLIC "--target=x86_64-pc-windows-gnu"
            PUBLIC -Wno-deprecated-enum-compare-conditional
        )
    endif()
endforeach()
