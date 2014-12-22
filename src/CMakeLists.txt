include_directories(${SCRY_SOURCE_DIR}/src)
include_directories(${SCRY_SOURCE_DIR}/src/third_party/tinyobjloader)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SCRY_SOURCE_DIR}/bin)

file(GLOB_RECURSE INCLUDES "*.h" "*.cpp" "*.cc")

add_executable(scry main.cc ${INCLUDES})

target_link_libraries(scry ${CORELIBS})
