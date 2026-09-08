#!/usr/bin/env bash
set -e

echo "=== [1/3] Cleaning Old Build Artifacts ==="
rm -f build_test build_bench build_multi_bench *.o *.so

echo "=== [2/3] Compiling & Running Parser Unit Test ==="
gcc -O3 -Wall -Wextra -Werror -std=c11 -Iinclude tests/test_parser.c -lm -o build_test
./build_test

echo "=== [3/3] Compiling & Executing Big Data Multi-Solver Benchmark ==="
gcc -O3 -Wall -Wextra -Werror -std=c11 -Iinclude benchmarks/bench_multi_solvers.c -lm -o build_multi_bench
./build_multi_bench

echo "=== ALL LOCAL CHECKS PASSED GREEN ==="
