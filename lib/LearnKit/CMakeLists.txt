
add_Nitetrichor_library(LearnKit
        learn.cpp
        Version.c
        BTree.cpp
        Kmp.cpp
        WNQueue.c
        Redirect.cpp
)

set(WNLearn_VERSION_STRING "0.1") # which also set the macro.

configure_file(${CMAKE_SOURCE_DIR}/include/Nitetrichor/LearnKit/Version.inc.in
        ${CMAKE_BINARY_DIR}/include/Nitetrichor/LearnKit/Version.inc)

target_include_directories(LearnKit BEFORE
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/Nitetrichor>
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include/Nitetrichor>
        $<INSTALL_INTERFACE:include/Nitetrichor>
        )