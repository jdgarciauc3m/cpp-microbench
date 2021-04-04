cmake_minimum_required(VERSION 3.16)
project(cpp_microbench)

set(CMAKE_CXX_STANDARD 20)

find_package(fmt)
find_package(Boost)
find_package(Microsoft.GSL)

add_compile_options(-Wall -Wextra -Werror -pedantic -pedantic-errors)

add_subdirectory(gen-floats)