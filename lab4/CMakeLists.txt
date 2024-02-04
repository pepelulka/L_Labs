add_library(lab4_lib1 SHARED src/lib1.c)
add_library(lab4_lib2 SHARED src/lib2.c)

target_include_directories(lab4_lib1 PUBLIC include)
target_include_directories(lab4_lib2 PUBLIC include)

add_executable(lab4_main1 main1.cpp)
target_link_libraries(lab4_main1 PRIVATE lab4_lib1)

add_executable(lab4_main2 main2.cpp)
target_include_directories(lab4_main2 PRIVATE include)
