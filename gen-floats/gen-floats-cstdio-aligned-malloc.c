#include <errno.h>
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
    v[i] = 1.0F / (float) (i+1);
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
  fprintf(file, "%lu\n", n);// NOLINT
  for (size_t i = 0; i < n; ++i) {
    fprintf(file, "%.18f\n", v[i]);// NOLINT
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

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Wrong arguments\n");
    fprintf(stderr, "%s nelems\n", argv[0]);
    exit(-1); // NOLINT
  }
  size_t max_value = strtoull(argv[1], NULL, 10);
  if (errno == ERANGE) {
    fprintf(stderr, "Cannot convert value %s to integer\n", argv[1]);
    exit(-1); // NOLINT
  }

  // A small dis-alignment in heap
  char * p = malloc(3);
  p[0] = ' ';

  struct bench_result t = bench_cstdio(max_value);
  print_bench(&t);

  // Avoid allocation of p to be optimized out
  printf("%c", p[0]);

  free(p);

  return 0;
}
