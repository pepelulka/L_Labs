add_library(lab1_lib src/utils.cpp
                     src/parent.cpp)

target_include_directories(lab1_lib PUBLIC include)

add_executable(lab1_child src/child.cpp)
add_executable(lab1_main main.cpp)

target_link_libraries(lab1_child PUBLIC lab1_lib)
target_link_libraries(lab1_main PUBLIC lab1_lib)
