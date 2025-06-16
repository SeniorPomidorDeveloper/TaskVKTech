# Поиск зависимостей для тестов
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)    

SET(CMAKE_CXX_FLAGS "-g -fprofile-arcs -ftest-coverage -Wall -Wextra -g")
SET(CMAKE_C_FLAGS "-g -fprofile-arcs -ftest-coverage -Wall -Wextra -g")

# Тестовые цели
include(GoogleTest)
macro(add_test_executable name)
    add_executable(${name} ${ARGN})
    target_compile_options(${name} PRIVATE --coverage)  
    target_link_libraries(${name} PRIVATE
        TaskVKTech-lib
        GTest::gtest
        GTest::gmock
        GTest::gtest_main
    )
    gtest_discover_tests(${name})
endmacro()

add_test_executable(SetEventTest SetEventTest.cpp)
add_test_executable(MetricTest MetricTest.cpp)
add_test_executable(MetricManagerTest MetricManagerTest.cpp)