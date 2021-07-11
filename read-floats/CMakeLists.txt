include_directories(..)

link_libraries(fmt::fmt)

add_executable(read-floats-cstdio-malloc read-floats-cstdio-malloc.c)

add_executable(read-floats-cstdio-aligned-malloc read-floats-cstdio-aligned-malloc.c)

add_executable(read-floats-stream-aligned-array read-floats-stream-aligned-array.cpp)

add_executable(read-floats-stream-aligned-vector read-floats-stream-aligned-vector.cpp)
