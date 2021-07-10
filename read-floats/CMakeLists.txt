add_executable(read-floats-cstdio-malloc read-floats-cstdio-malloc.c)

add_executable(read-floats-stream-aligned-array read-floats-stream-aligned-array.cpp)
target_link_libraries(read-floats-stream-aligned-array fmt::fmt)

add_executable(read-floats-stream-aligned-vector read-floats-stream-aligned-vector.cpp)
target_link_libraries(read-floats-stream-aligned-vector fmt::fmt)