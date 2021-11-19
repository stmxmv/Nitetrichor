
project(Nitetrichor_tests)
add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Nitetrichor_Tests_run test.cpp)
target_link_libraries(Nitetrichor_Tests_run gtest gtest_main)