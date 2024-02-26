set(CXX_STANDARD 11)

add_executable(example example.cpp)

target_compile_options(example PRIVATE -std=c++17)
target_link_Libraries(example ${PROJECT_NAME})