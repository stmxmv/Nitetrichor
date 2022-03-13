add_Nitetrichor_tool(other
        main.cpp
)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
target_compile_options(other PRIVATE "-fobjc-arc")
endif()



set_target_properties(other
        PROPERTIES
        CXX_STANDARD 20
)

target_link_libraries(other
        PRIVATE
#        "-framework Foundation"
#        "-framework CoreGraphics"
        Core
        )
