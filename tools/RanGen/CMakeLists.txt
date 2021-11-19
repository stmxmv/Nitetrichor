add_Nitetrichor_tool(RanGen
        main.cpp
)




target_link_libraries(RanGen
        PRIVATE
        Core
        LearnKit
)

target_compile_options(RanGen
        PRIVATE
        -O0
        -fno-exceptions
        -fno-rtti
)

set_target_properties(RanGen
        PROPERTIES
        CXX_STANDARD 17
)