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
    read-floats/$1
  done >tmp.dat

  grep Initial tmp.dat > init.dat
  compute_stat init.dat "Initial"
  rm init.dat
  grep Allocation tmp.dat > alloc.dat
  compute_stat alloc.dat "Allocation"
  rm alloc.dat
  grep Reading tmp.dat > read.dat
  compute_stat read.dat "Reading"
  rm read.dat
  rm tmp.dat
  echo
}

run read-floats-cstdio-malloc
run read-floats-cstdio-aligned-malloc
run read-floats-stream-aligned-array
run read-floats-stream-array
run read-floats-stream-aligned-vector
run read-floats-stream-vector