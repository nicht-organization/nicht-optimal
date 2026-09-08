#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <math.h>
#include "nicht/solvers_meta.h"

void run_competition_entry(const char *name, MetaSolverFunc solver, const TSPInstance *inst, const SolverBudget *budget) {
    MetaSolution sol = {0};
    if (solver(inst, budget, &sol)) {
        printf("| %-20s | %12.2f | %12llu | %10.3f ms |\n",
               name, sol.cost, (unsigned long long)sol.evaluations, sol.execution_time_ms);
    }
}

int main(void) {
    TSPInstance instance;
    instance.dimension = 100;
    for (uint32_t i = 0; i < instance.dimension; i++) {
        for (uint32_t j = 0; j < instance.dimension; j++) {
            instance.weight_matrix[i * instance.dimension + j] = fabsf((float)i - (float)j);
        }
    }

    SolverBudget budget = { .max_time_ms = 100.0, .max_evaluations = 10000000 };

    printf("=== META-HEURISTIC COMPETITION RACE ===\n");
    printf("| %-20s | %-12s | %-12s | %-12s |\n", "Algorithm", "Final Cost", "Evaluations", "Time");
    printf("|----------------------|--------------|--------------|--------------|\n");

    run_competition_entry("2-Opt SIMD Local", solver_2opt_simd_run, &instance, &budget);

    return 0;
}
