cmake_minimum_required(VERSION 2.8)
project(SCRY)

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Boost REQUIRED)

set(CMAKE_CXX_FLAGS "-g -Wall")

add_definitions("-std=c++11")

add_subdirectory(src)
