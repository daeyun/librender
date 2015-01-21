cmake_minimum_required(VERSION 2.8.4)

# Set -DTEST=ON to build unit tests.
option(TEST "Build all tests." OFF)
option(DEBUG "Debug mode." OFF)

project(LIBRENDER)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(OpenGL REQUIRED)
find_package(Boost COMPONENTS program_options filesystem system REQUIRED)
find_package(YamlCpp REQUIRED)
find_package(Armadillo REQUIRED)
find_package(GLEW REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

set(CORELIBS ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${ARMADILLO_LIBRARIES} ${Boost_LIBRARIES} ${OPENGL_LIBRARY} ${YAMLCPP_LIBRARY})
set(INCLUDE_DIRS ${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

if (DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g -rdynamic")
    add_definitions(-DDEBUG)
else()
    set(CMAKE_CXX_FLAGS "-Ofast")
endif()

link_directories(${GLFW_LIBRARY_DIRS})

add_definitions(-std=c++11)

# Pass in the version string at compile time.
# From git:  git tag -a v0.1 -m 'version 0.1'
#find_package(Git)
#if(GIT_FOUND)
#    execute_process(
#        COMMAND ${GIT_EXECUTABLE} describe --abbrev=4 --dirty --always --tags --match "v[0-9]*"
#        OUTPUT_VARIABLE DESCRIBE_BUILD
#        OUTPUT_STRIP_TRAILING_WHITESPACE)
#    STRING(REGEX MATCH "[0-9]+\\.[0-9]+-?[a-zA-Z0-9-]*" VERSION_STRING ${DESCRIBE_BUILD})
#    add_definitions(-DVERSION=\"${VERSION_STRING}\")
#endif()

# Used by both src and test
include_directories(${LIBRENDER_SOURCE_DIR}/src)
include_directories(${LIBRENDER_SOURCE_DIR}/src/third_party/tinyobjloader)
include_directories(${INCLUDE_DIRS})

add_subdirectory(src)

if (TEST)
    # This adds 'project(gmock)'
    add_subdirectory(${LIBRENDER_SOURCE_DIR}/lib/gmock-1.7.0)
    add_subdirectory(test)

    enable_testing()
    add_test(NAME RunTests COMMAND runUnitTests)
endif()
