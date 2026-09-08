#!/usr/bin/env bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "=== [1/5] Cleaning Old Build Artifacts ==="
rm -rf build
mkdir -p build/bin

echo "=== [2/5] Compiling & Running TSPLIB Parser Unit Tests ==="
gcc -O3 -Wall -Wextra -Werror -std=c11 -Iinclude tests/test_parser.c -lm -o build/bin/test_parser
./build/bin/test_parser

echo "=== [3/5] Benchmark 1: Multi-Solver Big Data Race ==="
gcc -O3 -Wall -Wextra -Werror -std=c11 -Iinclude benchmarks/bench_multi_solvers.c src/meta_2opt_simd.c -lm -o build/bin/bench_multi_solvers
./build/bin/bench_multi_solvers

echo "=== [4/5] Benchmark 2: TSPLIB Parsing & Materialization ==="
gcc -O3 -Wall -Wextra -Werror -std=c11 -Iinclude benchmarks/bench_tsplib.c src/meta_2opt_simd.c -lm -o build/bin/bench_tsplib
./build/bin/bench_tsplib

echo "=== [5/5] Benchmark 3: Meta-Heuristic Competition Budget Race ==="
gcc -O3 -Wall -Wextra -Werror -std=c11 -Iinclude benchmarks/bench_competition.c src/meta_2opt_simd.c -lm -o build/bin/bench_competition
./build/bin/bench_competition

echo "================================================="
echo "=== ALL 3 BENCHMARKS & TESTS PASSED GREEN ==="
echo "================================================="
