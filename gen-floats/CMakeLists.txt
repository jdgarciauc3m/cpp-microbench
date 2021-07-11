add_executable(gen-floats-cstdio-malloc gen-floats-cstdio-malloc.c)

add_executable(gen-floats-cstdio-aligned-malloc gen-floats-cstdio-malloc.c)

add_executable(gen-floats-fmt-aligned-vector gen-floats-fmt-aligned-vector.cpp)
target_link_libraries(gen-floats-fmt-aligned-vector fmt::fmt)

add_executable(gen-floats-fmt-aligned-array gen-floats-fmt-aligned-array.cpp)
target_link_libraries(gen-floats-fmt-aligned-array fmt::fmt)