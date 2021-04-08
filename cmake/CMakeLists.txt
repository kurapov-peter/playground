cmake_minimum_required(VERSION 3.13.4 FATAL_ERROR)
project(FindMeThePython)

find_package(Python3 COMPONENTS Interpreter)
message(STATUS "Python interpreter ${Python3_EXECUTABLE}")

set(pyscript ${CMAKE_CURRENT_SOURCE_DIR}/hello.py)
set(out_result "Python version message")

add_custom_target(RunHello ALL DEPENDS ${out_result})
add_custom_command(
    OUTPUT ${out_result}
    COMMAND "${Python3_EXECUTABLE}" ${pyscript}
    DEPENDS ${pyscript}
)
