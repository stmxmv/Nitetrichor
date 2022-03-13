
include(WN)

macro(add_Nitetrichor_tool name)
    wn_add_executable(${name} ${ARGN})
    if(TARGET ${name})
        # may install here

        install(TARGETS ${name}
                LIBRARY DESTINATION lib/${Nitetrichor_LIBDIR_SUFFIX}
                ARCHIVE DESTINATION lib/${Nitetrichor_LIBDIR_SUFFIX}
                RUNTIME DESTINATION bin
                )

    else()
        add_custom_target(${name} ${ARGN})
    endif()
endmacro(add_Nitetrichor_tool name)


macro(add_Nitetrichor_library name)
    wn_add_library(${name} ${ARGN})
    if(TARGET ${name})

        # may link here

        install(TARGETS ${name} EXPORT ${name}-targets
                COMPONENT ${name}
                LIBRARY DESTINATION lib/${Nitetrichor_LIBDIR_SUFFIX}
                ARCHIVE DESTINATION lib/${Nitetrichor_LIBDIR_SUFFIX}
                RUNTIME DESTINATION bin
                INCLUDES DESTINATION include)

        install(EXPORT ${name}-targets
                FILE "${PROJECT_NAME}-${name}-targets.cmake"
                NAMESPACE ${PROJECT_NAME}::
                DESTINATION lib/cmake/${PROJECT_NAME}
                COMPONENT ${component}
                )
    else()
        add_custom_target(${name} ${ARGN})
    endif()
endmacro()

set(Nitetrichor_LIBDIR_SUFFIX "/Nitetrichor")