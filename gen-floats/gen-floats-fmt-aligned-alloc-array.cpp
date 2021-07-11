#include "bench_output.h"

#include <iostream>

int main(int argc, char ** argv) {
  if (argc!=2) {
    std::cerr << "Wrong arguments.\n";
    std::cerr << argv[0] << " elems\n"; // NOLINT
    exit(-1); // NOLINT
  }

  // A small dis-alignment in heap
  std::unique_ptr<char[]> p = std::make_unique<char[]>(3); // NOLINT
  p[0] = ' ';

  std::size_t max_elements = std::stoull(argv[1]); // NOLINT

  auto r = bench_fmt_output_alloc_array(max_elements);
  print_bench(r);

  // Avoid allocation of p to be optimized out
  std::cout << p[0] << "\n";
}