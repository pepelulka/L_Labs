add_library(lab2_lib src/det.cpp)

target_include_directories(lab2_lib PUBLIC include)

add_executable(lab2_main main.cpp)

target_link_libraries(lab2_main PUBLIC lab2_lib)
