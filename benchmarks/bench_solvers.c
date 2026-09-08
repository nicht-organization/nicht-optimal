#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../include/nicht/optimal.h"
#include "../include/nicht/solvers.h"

int main(void) {
    printf("=== Running nicht-optimal 2-Opt Solver Benchmark ===\n");

    /* Create 100-node synth TSP instance */
    TSPLIBInstance inst;
    inst.dimension = 100;
    snprintf(inst.name, sizeof(inst.name), "synth_100");

    srand(42); /* Fixed seed for benchmark reproducibility */
    for (uint32_t i = 0; i < inst.dimension; i++) {
        inst.nodes[i].x = (float)(rand() % 1000);
        inst.nodes[i].y = (float)(rand() % 1000);
    }

    /* Compute explicit 100x100 weight matrix */
    uint32_t n = inst.dimension;
    for (uint32_t i = 0; i < n; i++) {
        for (uint32_t j = 0; j < n; j++) {
            float dx = inst.nodes[i].x - inst.nodes[j].x;
            float dy = inst.nodes[i].y - inst.nodes[j].y;
            inst.weight_matrix[i * n + j] = sqrtf(dx * dx + dy * dy);
        }
    }

    TSPSolution sol;
    clock_t start = clock();
    tsp_solve_2opt(&inst, &sol, 1000000);
    clock_t end = clock();

    double elapsed_ms = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;

    printf("Instance           : %s (%u nodes)\n", inst.name, inst.dimension);
    printf("Final Tour Length  : %.2f\n", sol.tour_length);
    printf("2-Opt Iterations   : %llu\n", (unsigned long long)sol.iterations);
    printf("Solver Execution   : %.3f ms\n", elapsed_ms);
    printf("=== Benchmark Completed Successfully ===\n");

    return 0;
}
