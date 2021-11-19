add_Nitetrichor_tool(Learn++
        main.cpp
        temp.cpp
)


target_compile_definitions(Learn++ PRIVATE
        WN_NOT_OJ
)


target_link_libraries(Learn++
        PRIVATE
        Core
        LearnKit
)

target_compile_options(Learn++
        PRIVATE
        -O0
        -fno-exceptions
        -fno-rtti
)

set_target_properties(Learn++
        PROPERTIES
        CXX_STANDARD 14
)