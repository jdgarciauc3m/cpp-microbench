#ifndef CPP_MICROBENCH_BENCH_INTPUT_H
#define CPP_MICROBENCH_BENCH_INTPUT_H

#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <tuple>

#include <fmt/os.h>
#include <fmt/ostream.h>

#include "util/aligned_vector.hpp"
#include "util/alloc_array.hpp"
#include "util/trivial_aligned_array.hpp"
#include "util/plain_array.hpp"

static constexpr std::size_t max_elements = 100'000;

auto open_file() {
  using namespace std::string_literals;
  auto file_name = "bench_stdio.txt"s;
  std::ifstream file{file_name};
  file.sync_with_stdio(false);
  if (!file) {
    fmt::print(std::cerr, "Cannot open file {}\n", file_name);
    std::exit(-1); // NOLINT
  }
  return file;
}

template<typename container_type>
auto make_container(std::istream & file) {
  std::size_t n = 0;
  file >> n;
  if (!file) {
    fmt::print(std::cerr, "Cannot read count from file\n");
    std::exit(-1); // NOLINT
  }

  container_type v(n);

  return v;
}

template <typename T>
struct is_vector_t : std::false_type {};

template <typename T, typename A>
struct is_vector_t<std::vector<T,A>> : std::true_type {};

template <typename T>
concept vector_like = is_vector_t<T>::value;

template <vector_like vector_type>
auto make_container(std::istream & is) {
  std::size_t n = 0;
  is >> n;
  if (!is) {
    fmt::print(std::cerr, "Cannot read count from file\n");
    std::exit(-1); // NOLINT
  }

  vector_type v;
  v.reserve(n);

  return v;
}

void stream_input(std::istream & is, auto & v) {
  for (std::size_t i = 0; i<v.size(); ++i) {
    is >> v[i];
    if (!is) {
      fmt::print(std::cerr, "Cannot read data item [{}] from file\n", i);
      exit(-1); // NOLINT
    }
  }

  std::cout << v[v.size()-1] << '\n';
}

template <vector_like vector_type>
void stream_input(std::istream & is, vector_type & v) {
  typename vector_type::value_type x;
  while (is >> x) {
    v.push_back(x);
  }
  if (!is.eof()) {
    fmt::print(std::cerr, "Cannot read data item [{}] from file\n", v.size());
    exit(-1); // NOLINT
  }

  std::cout << v[v.size()-1] << '\n';
}

template <typename container_type>
auto bench_stream() {
  using namespace std::chrono;

  auto t1 = high_resolution_clock::now();
  auto file = open_file();
  auto v = make_container<container_type>(file);
  auto t2 = high_resolution_clock::now();
  stream_input(file,v);
  auto t3 = high_resolution_clock::now();

  return std::tuple {__func__, t2 - t1, t3 - t2};
}

void print_bench(auto t) {
  using namespace std::chrono;
  auto d1 = duration_cast<microseconds>(std::get<1>(t));
  auto d2 = duration_cast<microseconds>(std::get<2>(t));
  fmt::print("\nVersion: {}\n", std::get<0>(t));
  fmt::print("Allocation {} us\n", d1.count());
  fmt::print("Reading {} us\n", d2.count());
}

#endif//CPP_MICROBENCH_BENCH_INTPUT_H
