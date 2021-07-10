#ifndef CPP_MICROBENCH_BENCH_INTPUT_H
#define CPP_MICROBENCH_BENCH_INTPUT_H

#include <string>
#include <fstream>
#include <iostream>
#include <chrono>

#include <fmt/os.h>
#include <fmt/ostream.h>

#include "trivial_array.h"
#include "dynamic_vector.h"

static constexpr std::size_t max_elements = 100'000;

auto stream_input_array() {
  using namespace std::string_literals;
  auto file_name = "bench_stdio.txt"s;
  std::ifstream file{file_name};
  file.sync_with_stdio(false);
  if (!file) {
    fmt::print(std::cerr, "Cannot open file {}\n", file_name);
    std::exit(-1); // NOLINT
  }

  std::size_t n = 0;
  file >> n;
  if (!file) {
    fmt::print(std::cerr, "Cannot read count from file {}\n", file_name);
    std::exit(-1); // NOLINT
  }

  trivial_dynamic_array<float> v{n};

  for (std::size_t i = 0; i<n; ++i) {
    file >> v[i];
    if (!file) {
      fmt::print(std::cerr, "Cannot read data item [{}] from file {}\n", i, file_name);
      exit(-1); // NOLINT
    }
  }

  std::cout << v[n-1] << '\n';

  return v;
}

auto stream_input_vector() {
  using namespace std::string_literals;
  auto file_name = "bench_stdio.txt"s;
  std::ifstream file{file_name};
  file.sync_with_stdio(false);
  if (!file) {
    fmt::print(std::cerr, "Cannot open file {}\n", file_name);
    std::exit(-1); // NOLINT
  }

  std::size_t n = 0;
  file >> n;
  if (!file) {
    fmt::print(std::cerr, "Cannot read count from file {}\n", file_name);
    std::exit(-1); // NOLINT
  }

  dynamic_vector<float> v(n);

  for (std::size_t i = 0; i<n; ++i) {
    file >> v[i];
    if (!file) {
      fmt::print(std::cerr, "Cannot read data item [{}] from file {}\n", i, file_name);
      exit(-1); // NOLINT
    }
  }

  std::cout << v[n-1] << '\n';

  return v;
}

auto bench_stream_input_array() {
  using namespace std::chrono;

  auto t1 = high_resolution_clock::now();
  auto t2 = high_resolution_clock::now();
  auto v = stream_input_array();
  auto t3 = high_resolution_clock::now();

  return std::tuple {__func__, t2 - t1, t3 - t2};
}

auto bench_stream_input_vector() {
  using namespace std::chrono;

  auto t1 = high_resolution_clock::now();
  auto t2 = high_resolution_clock::now();
  auto v = stream_input_vector();
  auto t3 = high_resolution_clock::now();

  return std::tuple {__func__, t2 - t1, t3 - t2};
}

void print_bench(auto t) {
  using namespace std::chrono;
  auto d1 = duration_cast<microseconds>(std::get<1>(t));
  auto d2 = duration_cast<microseconds>(std::get<2>(t));
  fmt::print("\nVersion: {}\n", std::get<0>(t));
  fmt::print("Generation {} us\n", d1.count());
  fmt::print("Reading {} us\n", d2.count());
}

#endif//CPP_MICROBENCH_BENCH_INTPUT_H
