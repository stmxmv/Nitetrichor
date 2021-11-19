
function(wn_check_source_file_list)
    cmake_parse_arguments(ARG "" "SOURCE_DIR" "" ${ARGN})
    foreach(l ${ARG_UNPARSED_ARGUMENTS})
        get_filename_component(fp ${l} REALPATH)
        list(APPEND listed ${fp})
    endforeach()

    if(ARG_SOURCE_DIR)
        file(GLOB globbed "${ARG_SOURCE_DIR}/*.c" "${ARG_SOURCE_DIR}/*.cpp")
    else()
        file(GLOB globbed *.c *.cpp)
    endif()

    foreach(g ${globbed})
        get_filename_component(fn ${g} NAME)
        if(ARG_SOURCE_DIR)
            set(entry "${g}")
        else()
            set(entry "${fn}")
        endif()
        get_filename_component(gp ${g} REALPATH)

        # Don't reject hidden files. Some editors create backups in the
        # same directory as the file.
        if (NOT "${fn}" MATCHES "^\\.")
            list(FIND WN_OPTIONAL_SOURCES ${entry} idx)
            if( idx LESS 0 )
                list(FIND listed ${gp} idx)
                if( idx LESS 0 )
                    if(ARG_SOURCE_DIR)
                        set(fn_relative "${ARG_SOURCE_DIR}/${fn}")
                    else()
                        set(fn_relative "${fn}")
                    endif()
                    message(SEND_ERROR "Found unknown source file ${fn_relative}
                    Please update ${CMAKE_CURRENT_LIST_FILE}\n")
                endif()
            endif()
        endif()
    endforeach()
endfunction(wn_check_source_file_list)


function(wn_process_sources OUT_VAR)

    cmake_parse_arguments(ARG "PARTIAL_SOURCES_INTENDED" "" "ADDITIONAL_HEADERS;ADDITIONAL_HEADER_DIRS" ${ARGN})
    set(sources ${ARG_UNPARSED_ARGUMENTS})
    if (NOT ARG_PARTIAL_SOURCES_INTENDED)
        wn_check_source_file_list(${sources})
    endif()
    # This adds .td and .h files to the Visual Studio solution:
    add_td_sources(sources)
    find_all_header_files(hdrs "${ARG_ADDITIONAL_HEADER_DIRS}")
    if (hdrs)
        set_source_files_properties(${hdrs} PROPERTIES HEADER_FILE_ONLY ON)
    endif()
    set_source_files_properties(${ARG_ADDITIONAL_HEADERS} PROPERTIES HEADER_FILE_ONLY ON)
    list(APPEND sources ${ARG_ADDITIONAL_HEADERS} ${hdrs})

    set( ${OUT_VAR} ${sources} PARENT_SCOPE )
endfunction(wn_process_sources)

macro(add_td_sources srcs)
    file(GLOB tds *.td)
    if( tds )
        source_group("TableGen descriptions" FILES ${tds})
        set_source_files_properties(${tds} PROPERTIES HEADER_FILE_ONLY ON)
        list(APPEND ${srcs} ${tds})
    endif()
endmacro(add_td_sources)

function(find_all_header_files hdrs_out additional_headerdirs)
    add_header_files_for_glob(hds *.h)
    list(APPEND all_headers ${hds})

    foreach(additional_dir ${additional_headerdirs})
        add_header_files_for_glob(hds "${additional_dir}/*.h")
        list(APPEND all_headers ${hds})
        add_header_files_for_glob(hds "${additional_dir}/*.inc")
        list(APPEND all_headers ${hds})
    endforeach(additional_dir)

    set( ${hdrs_out} ${all_headers} PARENT_SCOPE )
endfunction(find_all_header_files)

function(add_header_files_for_glob hdrs_out glob)
    file(GLOB hds ${glob})
    set(filtered)
    foreach(file ${hds})
        # Explicit existence check is necessary to filter dangling symlinks
        # out.  See https://bugs.gentoo.org/674662.
        if(EXISTS ${file})
            list(APPEND filtered ${file})
        endif()
    endforeach()
    set(${hdrs_out} ${filtered} PARENT_SCOPE)
endfunction(add_header_files_for_glob)


function(wn_add_library name)
    cmake_parse_arguments(ARG
            "MODULE;SHARED;STATIC"
            ""
            "ADDITIONAL_HEADERS"
            ${ARGN})
    if(ARG_ADDITIONAL_HEADERS)
        # Pass through ADDITIONAL_HEADERS.
        set(ARG_ADDITIONAL_HEADERS ADDITIONAL_HEADERS ${ARG_ADDITIONAL_HEADERS})
    endif()
    wn_process_sources(ALL_FILES ${ARG_UNPARSED_ARGUMENTS} ${ARG_ADDITIONAL_HEADERS})
    if(ARG_MODULE)
        add_library(${name} MODULE ${ALL_FILES})
    elseif(ARG_SHARED)
#        add_windows_version_resource_file(ALL_FILES ${ALL_FILES})
        add_library(${name} SHARED ${ALL_FILES})
    else()
        add_library(${name} STATIC ${ALL_FILES})
    endif()

    set_target_properties(${name}
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
            ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
            )

endfunction(wn_add_library)

function(wn_add_executable name)
    add_executable(${name} ${ARGN})

    set_target_properties(${name}
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
            ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
            )
endfunction(wn_add_executable)