#include "aligned_cont.h"
#include "bench_output.h"

#include <chrono>
#include <iostream>


int main() {
  // A small dis-alignment in heap
  std::unique_ptr<char[]> p = std::make_unique<char[]>(3); // NOLINT
  p[0] = 'z';

  auto r = bench_fmt_output_vector(max_elements);
  print_bench(r);

  // Avoid allocation of p to be optimized out
  std::cerr << p[0];
}