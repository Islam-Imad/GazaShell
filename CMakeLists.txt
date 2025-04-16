cmake_minimum_required(VERSION 3.5)
project(Shell)

# set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_executable(wish main.c)
target_link_libraries(wish PRIVATE shell_lib)

# Apply -Wall flag
target_compile_options(wish PRIVATE -Wall)
