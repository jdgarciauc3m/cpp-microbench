cmake_minimum_required(VERSION 3.17)
project(cpp_microbench)

set(CMAKE_CXX_STANDARD 20)

find_package(fmt)

add_compile_options(-Wall -Wextra -Werror -pedantic -pedantic-errors)

add_subdirectory(gen-floats)