cmake_minimum_required(VERSION 2.8)
list(APPEND CMAKE_CXX_FLAGS "-pthread -std=c++11 ${CMAKE_CXX_FLAGS} -g")
add_executable(joy main.cpp joystick.cpp)
