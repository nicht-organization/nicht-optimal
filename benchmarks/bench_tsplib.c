#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "nicht/tsplib_parser.h"

int main(void) {
    printf("=== BENCHMARK: TSPLIB File Buffer Parser & Matrix Materialization ===\n");
    
    /* Mock TSPLIB string buffer */
    const char *tsp_data = 
        "NAME : kroA100\n"
        "TYPE : TSP\n"
        "DIMENSION : 5\n"
        "EDGE_WEIGHT_TYPE : EUC_2D\n"
        "NODE_COORD_SECTION\n"
        "1 288.0 149.0\n"
        "2 288.0 1299.0\n"
        "3 2712.0 1420.0\n"
        "4 1230.0 800.0\n"
        "5 1500.0 200.0\n"
        "EOF\n";

    TSPInstance inst;
    uint64_t iterations = 100000;
    
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (uint64_t i = 0; i < iterations; i++) {
        tsplib_parse_buffer(tsp_data, &inst);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed_ms = (end.tv_sec - start.tv_sec) * 1000.0 + 
                         (end.tv_nsec - start.tv_nsec) / 1000000.0;

    printf("Parsed %llu instances in %.3f ms (%.2f ops/sec)\n",
           (unsigned long long)iterations, elapsed_ms, 
           (double)iterations / (elapsed_ms / 1000.0));

    return 0;
}
