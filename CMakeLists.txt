cmake_minimum_required(VERSION 3.13.4)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    project(Nitetrichor)

    set(Nitetrichor_BUILT_STANDALONE 1)


    list(APPEND CMAKE_MODULE_PATH
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")


    if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR AND NOT MSVC_IDE)

        message(FATAL_ERROR "In-source builds are not allowed.")

    endif()


    message("C++ Compiler is ${CMAKE_CXX_COMPILER_ID} version ${CMAKE_CXX_COMPILER_VERSION} (at ${CMAKE_CXX_COMPILER})")
    message("C Compiler is ${CMAKE_C_COMPILER_ID} version ${CMAKE_C_COMPILER_VERSION} (at ${CMAKE_C_COMPILER})")


    option(WN_MEM_CHECK "enable sanitize to check memory leak" OFF)
    if (WN_MEM_CHECK)
        set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -fsanitize=address")
        set (CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -fsanitize=address")
    endif ()


    include(Nitetrichor)

    include_directories(BEFORE
            ${CMAKE_CURRENT_BINARY_DIR}/include
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            )

    add_subdirectory(lib)
    add_subdirectory(tools)
    add_subdirectory(test)

endif()
