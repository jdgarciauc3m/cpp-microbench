#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct bench_result {
  clock_t t1, t2, t3;
};

struct bench_result bench_cstdio(size_t n) {

  struct bench_result timing;
  timing.t1 = clock();

  const unsigned long long pad = 256;
  const unsigned long long line_size = 64;

  float * buffer = malloc(n * sizeof(float) + pad);
  if (buffer == NULL) {
    fprintf(stderr, "Cannot allocate memory [size=%lu]\n", n);
    exit(-1);// NOLINT
  }
  float * v = (float *) (((unsigned long long) buffer + pad) & ~(line_size - 1));

  clock_t alloc_time = clock();

  for (size_t i = 0; i < n; ++i) {
    v[i] = 1.0F / (float) i;
  }

  timing.t2 = clock();

  double d1 = 1000000.0 * (alloc_time - timing.t1) / CLOCKS_PER_SEC;// NOLINT
  double d2 = 1000000.0 * (timing.t2 - alloc_time) / CLOCKS_PER_SEC;// NOLINT
  printf("Create %lf us\n", d1);
  printf("Fill %lf us\n", d2);

  FILE * file = fopen("bench_stdio.txt", "w");
  if (file == NULL) {
    fprintf(stderr, "Cannot open file %s for output\n", "bench_stdio.txt");
    exit(-1);// NOLINT
  }
  fprintf(file, "%lu", n);// NOLINT
  for (size_t i = 0; i < n; ++i) {
    fprintf(file, "%.18f", v[i]);// NOLINT
  }
  fclose(file);
  free(buffer);

  timing.t3 = clock();
  return timing;
}

void print_bench(const struct bench_result * t) {
  double d1 = 1000000.0 * (t->t2 - t->t1) / CLOCKS_PER_SEC;// NOLINT
  double d2 = 1000000.0 * (t->t3 - t->t2) / CLOCKS_PER_SEC;// NOLINT
  printf("\nVersion: bench_cstdio\n");
  printf("Generation %lf us\n", d1);
  printf("Writing %lf us\n", d2);
}

int main() {
  const int max_value = 100000;
  struct bench_result t = bench_cstdio(max_value);
  print_bench(&t);
  return 0;
}
