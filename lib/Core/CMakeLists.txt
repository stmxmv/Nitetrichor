add_Nitetrichor_library(Core
        Log.cpp
        Notification.cpp
        Basic.c
        UUID.cpp
        )


set(Nitetrichor_VERSION_STRING "0.1") # which also set the macro.

configure_file(${CMAKE_SOURCE_DIR}/include/Nitetrichor/Core/Version.inc.in
        ${CMAKE_BINARY_DIR}/include/Nitetrichor/Core/Version.inc)

target_include_directories(Core BEFORE
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/Nitetrichor>
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include/Nitetrichor>
        $<INSTALL_INTERFACE:include/Nitetrichor>
        )