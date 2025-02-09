cmake_minimum_required(VERSION 3.10)

project(MultithreadingDemo)

set(CMAKE_CXX_STANDARD 11)

# add_executable(1 第二节/第二节/第二节.cpp)
add_executable(class2 demo/class2.cpp)

target_link_libraries(class2 pthread)