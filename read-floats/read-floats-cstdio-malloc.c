#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct bench_result {
  clock_t t1, t2, t3, t4;
};

struct bench_result bench_cstdio() {

  struct bench_result timing;
  timing.t1 = clock();

  FILE * file = fopen("bench_stdio.txt", "r");
  if (file == NULL) {
    fprintf(stderr, "Cannot open file %s for input\n", "bench_stdio.txt");
    exit(-1); // NOLINT
  }
  size_t n = 0;
  int res = fscanf(file, "%lu", &n); // NOLINT
  if (res != 1) {
    fprintf(stderr, "Cannot read count from file %s\n", "bench_stdio.txt");
    exit(-1); // NOLINT
  }

  timing.t2 = clock();

  float * v = malloc(n * sizeof(float));
  if (v == NULL) {
    fprintf(stderr, "Cannot allocate memory [size=%lu]\n", n);
    exit(-1);// NOLINT
  }

  timing.t3 = clock();

  double d1 = 1000000.0 * (timing.t2 - timing.t1) / CLOCKS_PER_SEC;// NOLINT
  printf("Create %lf us\n", d1);

  for (size_t i = 0; i < n; ++i) {
    res = fscanf(file, "%f", &v[i]);// NOLINT
    if (res != 1) {
      fprintf(stderr, "Cannot read data index %lu from file %s\n", n, "bench_stdio.txt");
      exit(-1); // NOLINT
    }
  }
  fclose(file);
  free(v);

  timing.t4 = clock();
  return timing;
}

void print_bench(const struct bench_result * t) {
  double d1 = 1000000.0 * (t->t2 - t->t1) / CLOCKS_PER_SEC;// NOLINT
  double d2 = 1000000.0 * (t->t3 - t->t2) / CLOCKS_PER_SEC;// NOLINT
  double d3 = 1000000.0 * (t->t4 - t->t3) / CLOCKS_PER_SEC;// NOLINT
  printf("\nVersion: bench_cstdio\n");
  printf("Initial %lf us\n", d1);
  printf("Allocation %lf us\n", d2);
  printf("Reading %lf us\n", d3);
}

int main() {
  // A small dis-alignment in heap
  char * p = malloc(3);
  p[0] = ' ';

  struct bench_result t = bench_cstdio();
  print_bench(&t);

  // Avoid allocation of p to be optimized out
  printf("%c", p[0]);
  free(p);

  return 0;
}
