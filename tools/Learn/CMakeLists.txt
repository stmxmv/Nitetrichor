add_Nitetrichor_tool(Learn
        main.c
)


target_compile_definitions(Learn PRIVATE
        WN_NOT_OJ
)

target_compile_options(Learn PRIVATE
        -O0
)

target_link_libraries(Learn
        PRIVATE
        Core
        LearnKit
)