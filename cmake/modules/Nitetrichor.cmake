
include(WN)

macro(add_Nitetrichor_tool name)
    wn_add_executable(${name} ${ARGN})
    if(TARGET ${name})
        # may install here

        
    else()
        add_custom_target(${name} ${ARGN})
    endif()
endmacro(add_Nitetrichor_tool name)


macro(add_Nitetrichor_library name)

    wn_add_library(${name} ${ARGN})

    if(TARGET ${name})
        # may link here

        install(TARGETS ${name}
                COMPONENT ${name}
                LIBRARY DESTINATION lib${Nitetrichor_LIBDIR_SUFFIX}
                ARCHIVE DESTINATION lib${Nitetrichor_LIBDIR_SUFFIX}
                RUNTIME DESTINATION bin)
    else()
        add_custom_target(${name})
    endif()
endmacro()

set(Nitetrichor_LIBDIR_SUFFIX "/Nitetrichor")