cmake_minimum_required(VERSION 3.10)

project(MultithreadingDemo)

set(CMAKE_CXX_STANDARD 11)

# add_executable(1 第二节/第二节/第二节.cpp)
add_executable(class2 demo/class2.cpp)
add_executable(class3 demo/class3.cpp)
add_executable(class4 demo/class4.cpp)
add_executable(class5 demo/class5.cpp)

target_link_libraries(class2 pthread)
target_link_libraries(class3 pthread)
target_link_libraries(class4 pthread)
target_link_libraries(class5 pthread)