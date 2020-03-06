cmake_minimum_required(VERSION 3.15)

project(bitonic)

set(EXECUTABLE bitonic)

# Building explicitly for now
add_custom_target(${EXECUTABLE} ALL 
    COMMAND ${CMAKE_CXX_COMPILER} -fsycl ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp -o ${EXECUTABLE}
    COMMENT "Building main explicitly with clang++"
    VERBATIM
)

#add_executable(${EXECUTABLE} main.cpp)
#target_compile_options(${EXECUTABLE} PRIVATE-fsycl -Werror -Wall)
#target_link_libraries(${EXECUTABLE} sycl)
