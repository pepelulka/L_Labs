add_library(alloc1 src/allocator1.cpp)
target_include_directories(alloc1 PUBLIC include)

add_library(alloc2 src/allocator2.cpp)
target_include_directories(alloc2 PUBLIC include)

add_executable(cp_main src/timer.cpp main.cpp)
target_link_libraries(cp_main PRIVATE alloc1 alloc2)