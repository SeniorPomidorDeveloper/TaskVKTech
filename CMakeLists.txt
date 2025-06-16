cmake_minimum_required(VERSION 3.15)
project(TaskVKTech LANGUAGES CXX)

# Автоматическая инициализация пакетов
include(FetchContent)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    $<$<CONFIG:Debug>:-g>
)

include(CTest)
add_subdirectory(src)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

include(FeatureSummary)
feature_summary(WHAT ALL)