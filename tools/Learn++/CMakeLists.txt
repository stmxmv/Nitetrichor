add_Nitetrichor_tool(Learn++
        main.cpp
        temp.cpp
)


target_compile_definitions(Learn++ PRIVATE
        WN_NOT_OJ
)



find_package(Boost 0.1 REQUIRED timer)
if(Boost_FOUND)
    message("Found Boost ${Boost_VERSION_STRING}")
    target_include_directories(Learn++ PRIVATE ${Boost_INCLUDE_DIRS} "/Users/moah/XcodeProject/NItetrichor/WNITE/metal-cpp")

endif()

target_link_libraries(Learn++
        PRIVATE
        Core
        LearnKit
        ${Boost_LIBRARIES}
        "-framework Foundation"
        "-framework Cocoa"
)

target_compile_options(Learn++
        PRIVATE
        -O0
)
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    target_compile_options(Learn++ PRIVATE "-fobjc-arc")
endif()

set_target_properties(Learn++
        PROPERTIES
        CXX_STANDARD 17
)