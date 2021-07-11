#include "bench_input.h"

#include <iostream>
#include <memory>

int main() {
  // A small dis-alignment in heap
  std::unique_ptr<char[]> p = std::make_unique<char[]>(3); // NOLINT
  p[0] = ' ';

  auto r = bench_stream<aligned_vector<float>>();
  print_bench(r);

  // Avoid allocation of p to be optimized out
  std::cout << p[0] << "\n";
}