cmake_minimum_required(VERSION 3.10)

project(MultithreadingDemo)

set(CMAKE_CXX_STANDARD 11)

add_executable(class2 demo/class2.cpp)
add_executable(class3 demo/class3.cpp)
add_executable(class4 demo/class4.cpp)
add_executable(class5 demo/class5.cpp)
add_executable(class6 demo/class6.cpp)
add_executable(class7 demo/class7.cpp)
add_executable(class8 demo/class8.cpp)
add_executable(class9 demo/class9.cpp)
add_executable(class10 demo/class10.cpp)
add_executable(class11 demo/class11.cpp)
add_executable(class12 demo/class12.cpp)
add_executable(class13 demo/class13.cpp)
add_executable(demo13 demo/demo13.cpp)

target_link_libraries(class2 pthread)
target_link_libraries(class3 pthread)
target_link_libraries(class4 pthread)
target_link_libraries(class5 pthread)
target_link_libraries(class6 pthread)
target_link_libraries(class7 pthread)
target_link_libraries(class8 pthread)
target_link_libraries(class9 pthread)
target_link_libraries(class10 pthread)
target_link_libraries(class11 pthread)
target_link_libraries(class12 pthread)
target_link_libraries(class13 pthread)
target_link_libraries(demo13 pthread)
