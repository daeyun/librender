cmake_minimum_required(VERSION 2.8.4)
project(SCRY)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(Boost COMPONENTS program_options filesystem system REQUIRED)
find_package(YamlCpp REQUIRED)
find_package(Armadillo REQUIRED)

set(CMAKE_CXX_FLAGS "-g -Wall")

add_definitions(-std=c++11)

find_package(Git)
if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --abbrev=4 --dirty --always --tags --match "v[0-9]*"
        OUTPUT_VARIABLE DESCRIBE_BUILD
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    string(REGEX MATCH "[0-9]+\\.[0-9]+-?[a-zA-Z]*" VERSION_STRING ${DESCRIBE_BUILD})
    add_definitions(-DVERSION=\"${VERSION_STRING}\")
endif()

add_subdirectory(src)
