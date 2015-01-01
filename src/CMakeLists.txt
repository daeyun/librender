if (DEBUG OR TEST)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SCRY_SOURCE_DIR}/bin-dev)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SCRY_SOURCE_DIR}/bin)
endif()

# Get all files except for ./src/main.cc and make them a library.
file(GLOB_RECURSE INCLUDES "*.h" "*.cpp" "*.cc")
list(REMOVE_ITEM INCLUDES ${SCRY_SOURCE_DIR}/src/main.cc)

# Also used in ./test/CMakeLists.txt
add_library (scry_lib ${INCLUDES})

add_executable(scry main.cc)
target_link_libraries(scry scry_lib ${CORELIBS})
