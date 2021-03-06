#ifndef CPP_MICROBENCH_BENCH_OUTPUT_H
#define CPP_MICROBENCH_BENCH_OUTPUT_H

#include <string>
#include <chrono>
#include <fstream>
#include <iomanip>

#include <fmt/os.h>
#include <fmt/ostream.h>
#include <fmt/chrono.h>

#include <gsl/util>

#include "util/aligned_vector.hpp"
#include "util/alloc_array.hpp"
#include "util/trivial_aligned_array.hpp"
#include "util/plain_array.hpp"

template <typename array_type>
auto generate_array(std::size_t max) {
  using namespace std::chrono;
  auto t1 = high_resolution_clock::now();
  array_type v(max);
  auto t2 = high_resolution_clock::now();
  for (std::size_t i = 0; i < max; ++i) {
    v[i] = 1.0F / gsl::narrow_cast<float>(i+1);
  }
  auto t3 = high_resolution_clock::now();
  fmt::print("Create: {}\n", duration_cast<microseconds>(t2-t1));
  fmt::print("Fill: {}\n", duration_cast<microseconds>(t3-t2));
  return v;
}

template <typename vector_type>
auto generate_vector(std::size_t max) {
  using namespace std::chrono;
  auto t1 = high_resolution_clock::now();
  vector_type v;
  v.reserve(max);
  auto t2 = high_resolution_clock::now();
  for (std::size_t i = 0; i < max; ++i) {
    v.push_back(1.0F / gsl::narrow_cast<float>(i+1));
  }
  auto t3 = high_resolution_clock::now();
  fmt::print("Create: {}\n", duration_cast<microseconds>(t2-t1));
  fmt::print("Fill: {}\n", duration_cast<microseconds>(t3-t2));
  return v;
}

void fmt_output_vector(const auto & v) {
  auto file_name = std::string(static_cast<const char *>(__func__)) + ".txt";
  auto file = fmt::output_file(file_name);
  file.print("{}\n", v.size());

  for (const auto & x : v) {
    file.print("{:.18f}\n", x);
  }
}

void stream_output_vector(const auto & v) {
  auto file_name = std::string(static_cast<const char *>(__func__)) + ".txt";
  std::ofstream file{file_name};
  file << v.size() << '\n';

  for (const auto & x : v) {
    file << std::setprecision(18) << x << '\n';
  }
}

template <typename array_type>
auto bench_fmt_output_array(std::size_t n) {
  using namespace std::chrono;

  auto t1 = high_resolution_clock::now();
  auto v = generate_array<array_type>(n);
  auto t2 = high_resolution_clock::now();
  fmt_output_vector(v);
  auto t3 = high_resolution_clock::now();

  return std::tuple {__func__, t2 - t1, t3 - t2};
}

template <typename vector_type>
auto bench_fmt_output_vector(std::size_t n) {
  using namespace std::chrono;

  auto t1 = high_resolution_clock::now();
  auto v = generate_vector<vector_type>(n);
  auto t2 = high_resolution_clock::now();
  fmt_output_vector(v);
  auto t3 = high_resolution_clock::now();

  return std::tuple {__func__, t2 - t1, t3 - t2};
}

template <typename vector_type>
auto bench_stream_vector(std::size_t n) {
  using namespace std::chrono;

  auto t1 = high_resolution_clock::now();
  auto v = generate_vector<vector_type>(n);
  auto t2 = high_resolution_clock::now();
  stream_output_vector(v);
  auto t3 = high_resolution_clock::now();

  return std::tuple {__func__, t2 - t1, t3 - t2};
}


void print_bench(auto t) {
  using namespace std::chrono;
  auto d1 = duration_cast<microseconds>(std::get<1>(t));
  auto d2 = duration_cast<microseconds>(std::get<2>(t));
  fmt::print("\nVersion: {}\n", std::get<0>(t));
  fmt::print("Generation {} us\n", d1.count());
  fmt::print("Writing {} us\n", d2.count());
}

#endif//CPP_MICROBENCH_BENCH_OUTPUT_H
