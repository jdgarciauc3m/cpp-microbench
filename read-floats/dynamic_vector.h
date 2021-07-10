#ifndef CPP_MICROBENCH_DYNAMIC_VECTOR_H
#define CPP_MICROBENCH_DYNAMIC_VECTOR_H

#include <vector>

#include <boost/align/aligned_allocator.hpp>

template <typename T>
using dynamic_vector =
    std::vector<T,
                boost::alignment::aligned_allocator<T, alignment>>;

#endif//CPP_MICROBENCH_DYNAMIC_VECTOR_H
