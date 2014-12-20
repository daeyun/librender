include_directories(${SCRY_SOURCE_DIR}/src)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SCRY_SOURCE_DIR}/bin)

file(GLOB_RECURSE INCLUDES "*.h" "*.cpp" "*.cc")
add_executable(scry scry.cpp ${INCLUDES})

set(CORELIBS ${BOOST_LIBRARY} ${GLUT_LIBRARY} ${OPENGL_LIBRARY} m)
target_link_libraries(scry ${CORELIBS})
