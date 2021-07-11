#ifndef CPP_MICROBENCH_BENCH_OUTPUT_H
#define CPP_MICROBENCH_BENCH_OUTPUT_H

#include <string>
#include <chrono>

#include <fmt/os.h>
#include <fmt/ostream.h>

void fmt_output_vector(const auto & v) {
  auto file_name = std::string(static_cast<const char *>(__func__)) + ".txt";
  auto file = fmt::output_file(file_name);
  file.print("{}\n", v.size());

  for (const auto & x : v) {
    file.print("{:.18f}\n", x);
  }
}

auto bench_fmt_output_array(std::size_t n) {
  using namespace std::chrono;

  auto t1 = high_resolution_clock::now();
  auto v = generate_trivial_array<float>(n);
  auto t2 = high_resolution_clock::now();
  fmt_output_vector(v);
  auto t3 = high_resolution_clock::now();

  return std::tuple {__func__, t2 - t1, t3 - t2};
}

auto bench_fmt_output_vector(std::size_t n) {
  using namespace std::chrono;

  auto t1 = high_resolution_clock::now();
  auto v = generate_vector<float>(n);
  auto t2 = high_resolution_clock::now();
  fmt_output_vector(v);
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
