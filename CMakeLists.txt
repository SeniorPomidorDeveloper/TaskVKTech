cmake_minimum_required(VERSION 3.10)
project(TaskVKTech)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Headers
set(HEADERS
    include/IEvent.hpp
    include/SetEvent.hpp
    include/SetEvent.cpp
    include/IMetric.hpp
    include/Metric.hpp
)

# Source files
set(SOURCES
    src/MetricManager.cpp
    src/MetricManagerBuilder.cpp
)

SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
SET(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")

# Enable testing
include(CTest)

add_executable(
    unit_tests
    tests/SetEventTest.cpp
    tests/MetricTest.cpp
    tests/MetricManagerTest.cpp
    ${SOURCES}
)

target_compile_options(unit_tests PRIVATE --coverage)  

target_link_libraries(
    unit_tests
    gtest
    gmock
    gtest_main
    pthread
    gcov
)

include(GoogleTest)
gtest_discover_tests(unit_tests)

# Print configuration summary
message(STATUS "")
message(STATUS "Generated with CMake version: ${CMAKE_VERSION}")
message(STATUS "")
message(STATUS "Project configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "")
message(STATUS "Build configuration:")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "")