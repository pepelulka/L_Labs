add_library(lab5_lib src/utils.cpp src/manager_base.cpp src/computer_base.cpp)

target_link_libraries(lab5_lib PUBLIC zmq)
target_include_directories(lab5_lib PUBLIC lib include)

add_executable(manager src/manager.cpp)
add_executable(computer src/computer.cpp)

target_link_libraries(manager PRIVATE lab5_lib)
target_link_libraries(computer PRIVATE lab5_lib)
