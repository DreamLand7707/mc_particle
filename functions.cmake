function(print_list list)
    foreach(list_item IN LISTS ${list})
        message(STATUS ${list_item})
    endforeach()
endfunction()

function(println message)
    message(STATUS ${message})
endfunction()

function(get_file_directory file_path out_var)
    string(FIND ${file_path} "/" pos REVERSE)

    if(pos EQUAL -1)
        set(temp1 "")
    else()
        string(SUBSTRING ${file_path} 0 ${pos} temp1)
    endif()

    set(${out_var} "${temp1}" PARENT_SCOPE)
endfunction()

function(get_first_line str out_var)
    string(FIND ${str} "\n" pos)

    if(pos EQUAL -1)
        set(temp1 ${str})
    else()
        string(SUBSTRING ${str} 0 ${pos} temp1)
    endif()

    set(${out_var} "${temp1}" PARENT_SCOPE)
endfunction()

function(open_register_target suffix)
    set(__INTERNAL__REGISTER_TRAGETS__${suffix} "" CACHE STRING "targets registered for unified processing")
endfunction()

function(clear_register_target suffix)
    set(__INTERNAL__REGISTER_TRAGETS__${suffix} "" CACHE STRING "targets registered for unified processing" FORCE)
endfunction()

function(get_register_target suffix out_var)
    set(${out_var} "${__INTERNAL__REGISTER_TRAGETS__${suffix}}" PARENT_SCOPE)
endfunction()

function(delete_register_target suffix)
    unset(__INTERNAL__REGISTER_TRAGETS__${suffix} CACHE)
endfunction()

function(add_register_target suffix target)
    set(_temp1 ${__INTERNAL__REGISTER_TRAGETS__${suffix}})
    list(FIND _temp1 ${target} _temp2)

    if(_temp2 EQUAL -1)
        list(APPEND _temp1 ${target})
        set(__INTERNAL__REGISTER_TRAGETS__${suffix} ${_temp1}
            CACHE STRING "targets registered for unified processing" FORCE)
    endif()

    set_register_target_properties(${target} NOT_DEFAULT_CXX_STANDARD OFF)
    set_register_target_properties(${target} NOT_DEFAULT_OUTPUT_DIRECTORY OFF)
    set_register_target_properties(${target} NOT_DEFAULT_INCLUDE_DIRECTORY OFF)
    set_register_target_properties(${target} NOT_DEFAULT_LIBRARY OFF)
    set_register_target_properties(${target} NOT_DEFAULT_DEFINITION OFF)
    set_register_target_properties(${target} NOT_DEFAULT_DEBUG_SIGN OFF)
    set_register_target_properties(${target} NOT_DEFAULT_COMPILE_OPTIONS OFF)
endfunction()

function(set_register_target_properties target ...)
    math(EXPR _temp1 "${ARGC}-1")

    foreach(I RANGE 1 ${_temp1} 2)
        math(EXPR II "${I}+1")
        set(property_name ${ARGV${I}})
        set(property_value ${ARGV${II}})
        set("__${target}__${property_name}" ${property_value}
            CACHE STRING "target: ${target} - key: ${property_name} - value: ${property_value}" FORCE)
    endforeach()
endfunction()

function(get_register_target_property target property value)
    set(${value} ${__${target}__${property}} PARENT_SCOPE)
endfunction()

function(clear_register_target_property target property)
    set(${__${target}__${property}} ${property_value}
        CACHE STRING "target: ${target} - key: ${property} - value: " FORCE)
endfunction()

function(delete_register_target_property target property)
    unset(${__${target}__${property}} CACHE)
endfunction()

function(find_proj_flag_file projs_relative_path proj_flag out_proj_flag_files)
    file(GLOB_RECURSE _temp RELATIVE ${projs_relative_path} ${projs_relative_path}/*/${proj_flag})
    set(${out_proj_flag_files} "${_temp}" PARENT_SCOPE)
endfunction()

function(make_all_proj_filemap projs_relative_path proj_flag_files all_projs_file proj_flag out_projs)
    set(proj_content "")
    set(I 0)

    foreach(proj_item IN LISTS proj_flag_files)
        file(READ ${projs_relative_path}/${proj_item} content)
        get_first_line(${content} varb)

        if(${varb} STREQUAL "")
            continue()
        endif()

        if(${I} EQUAL 0)
            file(WRITE ${all_projs_file} ${varb}\n)
        else()
            file(APPEND ${all_projs_file} ${varb}\n)
        endif()

        list(APPEND proj_content ${varb})
        get_file_directory(${proj_item} proj_item_path)
        set(__${proj_flag}_${varb}_path ${projs_relative_path}/${proj_item_path}
            CACHE STRING "project path: ${proj_flag} - ${varb}" FORCE)

        set(I 1)
    endforeach()

    set(${out_projs} "${proj_content}" PARENT_SCOPE)
endfunction()

function(make_dependencies_parser makefile_relative_path ccompiler cxxcompiler code_name output_execuable_name)
    execute_process(
        COMMAND make
        -C ${makefile_relative_path}
        C_COMPLER=${ccompiler}
        CXX_COMPLER=${cxxcompiler}
        OUTPUT_EXE=${output_execuable_name}
        CODE_NAME=${code_name}
    )
endfunction()

function(parse_dependencies execuatble_relative_path executable_name dependencies_file custom_dependence_file all_proj_file out_sort_projs)
    execute_process(
        COMMAND ${execuatble_relative_path}/${executable_name} ${dependencies_file} ${custom_dependence_file} ${all_proj_file}
        OUTPUT_VARIABLE sort_projs
    )
    set(${out_sort_projs} "${sort_projs}" PARENT_SCOPE)
endfunction()

function(add_sorts_projects_directories proj_flag sort_projs)
    foreach(proj IN LISTS sort_projs)
        add_subdirectory(${__${proj_flag}_${proj}_path})
    endforeach()
endfunction()