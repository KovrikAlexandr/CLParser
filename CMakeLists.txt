cmake_minimum_required(VERSION 3.12)

project(
    CLParser
    VERSION 1.0
    LANGUAGES CXX
)

set(CXX_STANDARD 20)

set(BUILD_EXAMPLE TRUE)
set(BUILD_TESTS TRUE)

add_subdirectory(src)

if(${BUILD_EXAMPLE})
    add_subdirectory(example)
endif()

if(${BUILD_TESTS})
    enable_testing()
    add_subdirectory(tests)
endif()
