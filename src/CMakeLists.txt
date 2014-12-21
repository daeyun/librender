include_directories(${SCRY_SOURCE_DIR}/src)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SCRY_SOURCE_DIR}/bin)

file(GLOB_RECURSE INCLUDES "*.h" "*.cpp" "*.cc")
add_executable(scry main.cc ${INCLUDES})

set(CORELIBS ${ARMADILLO_LIBRARIES} ${Boost_LIBRARIES} ${GLUT_LIBRARY} ${OPENGL_LIBRARY} ${YAMLCPP_LIBRARY})
target_link_libraries(scry ${CORELIBS})
