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
    "${ROOT_dependency_executable_dir}"
)

parse_dependencies(
    "${ROOT_dependency_executable_dir}"
    "${ROOT_dependence_exe_name}"
    "${ROOT_dependencies_file}"
    "${ROOT_custom_dependencies_file}"
    "${ROOT_all_projs_file}"
    ROOT_sort_projs
)

if(NOT HOST_PYTHON_VAR_RANGE)
    set(HOST_PYTHON_VAR_RANGE 3.10...3.13)
endif()

open_register_target(ROOT)
clear_register_target(ROOT)

find_package(Boost CONFIG REQUIRED COMPONENTS filesystem iostreams xpressive locale)
find_package(Eigen3 CONFIG REQUIRED)
find_package(GSL REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(ICU REQUIRED COMPONENTS data i18n io test tu uc)
find_package(Python3 ${HOST_PYTHON_VAR_RANGE} COMPONENTS Interpreter Development NumPy REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
find_package(libxml2 CONFIG REQUIRED)
find_package(OpenCL REQUIRED)

pkg_check_modules(gmp REQUIRED IMPORTED_TARGET gmp)
pkg_check_modules(gmpxx REQUIRED IMPORTED_TARGET gmpxx)

add_sorts_projects_directories(
    "${ROOT_project_flag}"
    "${ROOT_sort_projs}"
)

if(${CMAKE_C_COMPILER_ID} STREQUAL "GNU")
    set(CMAKE_C_FLAGS_DEBUG
        "-O0 -gdwarf-4"
        CACHE STRING "Additional C flags for debug mode" FORCE)
    set(CMAKE_CXX_FLAGS_DEBUG
        "-O0 -gdwarf-4"
        CACHE STRING "Additional Cxx flags for debug mode" FORCE)
    set(CMAKE_C_FLAGS_RELWITHDEBINFO
        "-O2 -DNDEBUG -gdwarf-4"
        CACHE STRING "Additional C flags for relWithInfo mode" FORCE)
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO
        "-O2 -DNDEBUG -gdwarf-4"
        CACHE STRING "Additional Cxx flags for relWithInfo mode" FORCE)
endif()

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
        get_register_target_property(${target} TARGET_NEED_DETACHED_DIR temp2)
        set_target_properties(${target} PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY "${ROOT_DIR}/Output/Lib/$<$<BOOL:${temp2}>:${target}>"
            RUNTIME_OUTPUT_DIRECTORY_DEBUG "${ROOT_DIR}/Output/Debug/$<$<BOOL:${temp2}>:${target}>"
            RUNTIME_OUTPUT_DIRECTORY_RELEASE "${ROOT_DIR}/Output/Release/$<$<BOOL:${temp2}>:${target}>"
            ARCHIVE_OUTPUT_DIRECTORY "${ROOT_DIR}/Output/ARCH/$<$<BOOL:${temp2}>:${target}>"
        )
    endif()

    get_register_target_property(${target} NOT_DEFAULT_INCLUDE_DIRECTORY temp)

    if(NOT ${temp})
        target_include_directories(${target}
            PRIVATE ${ROOT_include_dir}
            PRIVATE ${Python3_INCLUDE_DIRS}
            PRIVATE ${Python3_NumPy_INCLUDE_DIRS}
        )
    endif()

    get_register_target_property(${target} NOT_DEFAULT_LIBRARY temp)

    if(NOT ${temp})
        target_link_libraries(${target} PRIVATE
            Eigen3::Eigen
            Boost::filesystem Boost::iostreams Boost::locale
            GSL::gsl GSL::gslcblas

            ginac
            cln
            PkgConfig::gmp PkgConfig::gmpxx

            glfw

            OpenGL::GL OpenGL::GLU

            icudt.dll icuin.dll icuio.dll icutest.dll icutu.dll icuuc.dll

            LibXml2::LibXml2

            OpenCL::OpenCL

            ${Python3_LIBRARY_RELEASE}
        )
    endif()

    get_register_target_property(${target} NOT_DEFAULT_DEFINITION temp)

    if(NOT ${temp})
        get_register_target_property(${target} TARGET_PROJECT_SOURCE_PATH temp2)
        target_compile_definitions(${target}
            PRIVATE UNICODE
            PRIVATE _UNICODE
            PRIVATE LOCAL=1
            PRIVATE "ROOT_DIR=\"${ROOT_DIR}\""
            PRIVATE "PROJECT_DIR=\"${temp2}\""
        )
    endif()

    get_register_target_property(${target} NOT_DEFAULT_DEBUG_SIGN temp)

    if(NOT ${temp})
        if(${CMAKE_BUILD_TYPE} EQUAL Debug)
            target_compile_options(${target}
                PRIVATE "-g"
            )
        endif()
    endif()

    get_register_target_property(${target} NOT_DEFAULT_COMPILE_OPTIONS temp)

    if(NOT ${temp})
        if(${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
            target_compile_options(${target}
                PRIVATE "--target=x86_64-pc-windows-gnu"
            )
        endif()

        target_compile_options(${target}
            PRIVATE -Wno-deprecated-enum-compare-conditional
        )
    endif()
endforeach()
