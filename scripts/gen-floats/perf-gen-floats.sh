#!/bin/sh


compute_stat() {
  echo -n $2
  cat $1 |
  awk 'BEGIN{
    sum = 0;
    sumsq = 0;
  }
  {
    sum +=$2;
    sumsq +=($2)^2;
    n++;
  }
  END {
    avg = sum/n;
    print ": " avg " std-dev: " sqrt(sumsq/n - avg * avg);
  }'
}

run() {
  echo $1 $2

  ITER=100

  for i in $(seq 1 ${ITER}); do
    gen-floats/$1 $2
  done >tmp.dat

  grep Generation tmp.dat > gen.dat
  compute_stat gen.dat "Generation"
  rm gen.dat
  grep Writing tmp.dat > write.dat
  compute_stat write.dat "Writing"
  rm write.dat
  rm tmp.dat
  echo
}

NITER=100000

run gen-floats-cstdio-malloc $NITER
run gen-floats-cstdio-aligned-malloc $NITER
run gen-floats-fmt-aligned-array $NITER
run gen-floats-fmt-aligned-alloc-array $NITER
run gen-floats-fmt-aligned-vector $NITER