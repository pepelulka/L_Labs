include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab1_test lab1_test.cpp)
target_link_libraries(lab1_test PRIVATE GTest::GTest lab1_lib)

add_executable(lab2_test lab2_test.cpp)
target_link_libraries(lab2_test PRIVATE GTest::GTest lab2_lib)

add_executable(lab3_test lab3_test.cpp)
target_link_libraries(lab3_test PRIVATE GTest::GTest lab3_lib)

add_executable(lab4_test1 lab4_test1.cpp)
add_executable(lab4_test2 lab4_test2.cpp)
target_link_libraries(lab4_test1 GTest::GTest lab4_lib1)
target_link_libraries(lab4_test2 GTest::GTest lab4_lib2)
