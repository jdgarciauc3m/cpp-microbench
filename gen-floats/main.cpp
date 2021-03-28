#include <cstdio>
#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <fmt/ostream.h>
#include <fmt/chrono.h>
#include <fmt/os.h>

auto bench_cstdio(int n)
{
  using namespace std::chrono;

  auto t1 = high_resolution_clock::now();

  auto *v = static_cast<float *>(malloc(n * sizeof(float)));
  for (int i = 0; i < n; ++i) {
    v[i] = 1.0f / static_cast<float>(n);
  }

  auto t2 = high_resolution_clock::now();

  auto file_name = std::string(__func__) + ".txt";
  std::FILE *file = std::fopen(file_name.c_str(), "w");
  fprintf(file, "%i", n);

  for (int i = 0; i < n; ++i) {
    fprintf(file, "%.18f", v[i]);
  }

  fclose(file);
  free(v);

  auto t3 = high_resolution_clock::now();

  return std::tuple{__func__, t2 - t1, t3 - t2};
}

auto bench_iostream(int n)
{
  using namespace std::chrono;

  auto t1 = high_resolution_clock::now();

  std::vector<float> v(n);
  for (int i = 0; i < n; ++i) {
    v[i] = 1.0f / static_cast<float>(n);
  }

  auto t2 = high_resolution_clock::now();

  auto file_name = std::string(__func__) + ".txt";
  std::ofstream file{file_name};
  file << n << "\n";

  file.setf(std::ios::fixed);
  file.precision(18);
  for (auto & x : v) {
    file << x << "\n";
  }

  auto t3 = high_resolution_clock::now();

  return std::tuple{__func__, t2 - t1, t3 - t2};
}

auto bench_fmt_cstdio(int n)
{
  using namespace std::chrono;

  auto t1 = high_resolution_clock::now();

  std::vector<float> v(n);
  for (int i = 0; i < n; ++i) {
    v[i] = 1.0f / static_cast<float>(n);
  }

  auto t2 = high_resolution_clock::now();

  auto file_name = std::string(__func__) + ".txt";
  FILE *file = std::fopen(file_name.c_str(), "w");
  fmt::print(file, "{}\n", n);

  for (auto & x : v) {
    fmt::print(file, "{:.18f}\n", x);
  }

  fclose(file);

  auto t3 = high_resolution_clock::now();

  return std::tuple{__func__, t2 - t1, t3 - t2};
}

auto bench_fmt_ofstream(int n)
{
  using namespace std::chrono;

  auto t1 = high_resolution_clock::now();

  std::vector<float> v(n);
  for (int i = 0; i < n; ++i) {
    v[i] = 1.0f / static_cast<float>(n);
  }

  auto t2 = high_resolution_clock::now();

  auto file_name = std::string(__func__) + ".txt";
  std::ofstream file{file_name};
  fmt::print(file, "{}\n", n);

  for (auto & x : v) {
    fmt::print(file, "{:.18f}\n", x);
  }

  auto t3 = high_resolution_clock::now();

  return std::tuple{__func__, t2 - t1, t3 - t2};
}


auto bench_fmt_output(int n)
{
  using namespace std::chrono;

  auto t1 = high_resolution_clock::now();

  std::vector<float> v(n);
  for (int i = 0; i < n; ++i) {
    v[i] = 1.0f / static_cast<float>(n);
  }

  auto t2 = high_resolution_clock::now();

  auto file_name = std::string(__func__) + ".txt";
  auto file = fmt::output_file(file_name);
  file.print("{}\n", n);

  for (auto & x : v) {
    file.print("{:.18f}\n", x);
  }

  auto t3 = high_resolution_clock::now();

  return std::tuple{__func__, t2 - t1, t3 - t2};
}

void print_bench(auto current, auto base)
{
  fmt::print("\nVersion: {}\n", std::get<0>(current));
  fmt::print("Generation {} [Speedup: {}]\n",
      std::get<1>(current),
      std::get<1>(base) * 100 / std::get<1>(current));
  fmt::print("Writing {} [Speedup: {}]\n",
      std::get<2>(current),
      std::get<2>(base) * 100 / std::get<2>(current));
}

int main()
{
  constexpr int max_value = 100'000;

  auto b1 = bench_cstdio(max_value);
  auto b2 = bench_iostream(max_value);
  auto b3 = bench_fmt_cstdio(max_value);
  auto b4 = bench_fmt_ofstream(max_value);
  auto b5 = bench_fmt_output(max_value);

  print_bench(b1, b1);
  print_bench(b2, b1);
  print_bench(b3, b1);
  print_bench(b4, b1);
  print_bench(b5, b1);
}
