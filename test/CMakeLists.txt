include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${LIBRENDER_SOURCE_DIR}/bin)

file(GLOB_RECURSE INCLUDES "test_*.cc")
add_executable(runUnitTests ${INCLUDES})

target_link_libraries(runUnitTests librender_lib gmock gmock_main gtest gtest_main ${CORELIBS})
