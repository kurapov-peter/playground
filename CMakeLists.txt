cmake_minimum_required(VERSION 3.2)

project(bitonic)

set(EXECUTABLE bitonic)
add_executable(${EXECUTABLE} main.cpp)
target_compile_options(${EXECUTABLE} PRIVATE -fsycl)
target_link_libraries(${EXECUTABLE} sycl)

