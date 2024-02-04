add_library(lab3_lib src/utils.cpp src/parent.cpp)
target_include_directories(lab3_lib PUBLIC include)

add_executable(lab3_main main.cpp)
add_executable(lab3_child src/child.cpp)

target_link_libraries(lab3_main PRIVATE lab3_lib)
target_link_libraries(lab3_child PRIVATE lab3_lib)
