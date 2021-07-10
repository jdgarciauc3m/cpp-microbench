add_executable(read-floats-cstdio-malloc read-floats-cstdio-malloc.c)

add_executable(read-floats-aligned-array read-floats-aligned-array.cpp)
target_link_libraries(read-floats-aligned-array fmt::fmt)

