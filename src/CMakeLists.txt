set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${LIBRENDER_SOURCE_DIR}/bin)

# Get all files except for ./src/main.cc and make them a library.
file(GLOB_RECURSE INCLUDES "*.h" "*.cpp" "*.cc")
list(REMOVE_ITEM INCLUDES ${LIBRENDER_SOURCE_DIR}/src/main.cc)

# Also used in ./test/CMakeLists.txt
add_library (librender_lib ${INCLUDES})

add_executable(render main.cc)
target_link_libraries(render librender_lib ${CORELIBS})
