add_library(${PROJECT_NAME} 
    CLParser.cpp
    Arguments.cpp
)

target_compile_options(${PROJECT_NAME} PUBLIC -std=c++20)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)