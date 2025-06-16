# Основная библиотека
add_library(TaskVKTech-lib STATIC
    MetricManager.cpp
    MetricManagerBuilder.cpp
)

target_include_directories(TaskVKTech-lib PUBLIC 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

# Автоматическое добавление заголовков
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS include/*.hpp)
target_sources(TaskVKTech-lib PRIVATE ${HEADERS})
set_target_properties(TaskVKTech-lib PROPERTIES
    CXX_CLANG_TIDY "clang-tidy"
    LINKER_LANGUAGE CXX
)