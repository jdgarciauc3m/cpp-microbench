#ifndef CPP_MICROBENCH_ALIGNED_VECTOR_HPP
#define CPP_MICROBENCH_ALIGNED_VECTOR_HPP

#include <vector>

#include <boost/align/aligned_allocator.hpp>

constexpr std::size_t alignment = 4096;

template <typename T>
using aligned_vector =
std::vector<T,
    boost::alignment::aligned_allocator<T, alignment>>;

#endif//CPP_MICROBENCH_ALIGNED_VECTOR_HPP
