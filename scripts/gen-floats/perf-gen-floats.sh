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
  echo $1

  ITER=100

  for i in $(seq 1 ${ITER}); do
    gen-floats/$1
  done >tmp.dat

  grep Generation tmp.dat > gen.dat
  compute_stat gen.dat "Generation"
  rm gen.dat
  grep Writing tmp.dat > write.dat
  compute_stat write.dat "Writing"
  rm write.dat
  rm tmp.dat
}

run gen-floats-cstdio-malloc
run gen-floats-cstdio-aligned-malloc
run gen-floats-fmt-aligned-array
run gen-floats-fmt-aligned-vector