#include <stdio.h>
#include "nicht/solvers_meta.h"

void run_competition(const char *solver_name, MetaSolverFunc solver, const TSPInstance *inst, const SolverBudget *budget) {
    MetaSolution sol = {0};
    if (solver(inst, budget, &sol)) {
        printf("| %-20s | %12.2f | %12llu | %10.3f ms |\n",
               solver_name,
               sol.cost,
               (unsigned long long)sol.evaluations,
               sol.execution_time_ms);
    } else {
        printf("| %-20s | FAILED       | N/A          | N/A          |\n", solver_name);
    }
}

int main(void) {
    /* Mock 100-node TSP Instance */
    TSPInstance instance;
    instance.dimension = 100;
    
    /* Populate metric matrix with synthetic coordinates */
    for (uint32_t i = 0; i < instance.dimension; i++) {
        for (uint32_t j = 0; j < instance.dimension; j++) {
            float diff = (float)i - (float)j;
            instance.weight_matrix[i * instance.dimension + j] = fabsf(diff);
        }
    }

    SolverBudget budget = {
        .max_time_ms = 100.0,         /* 100ms wall-clock limit */
        .max_evaluations = 10000000,
        .target_cost = 0.0
    };

    printf("=== nicht-optimal Meta-Heuristic Competition Benchmark ===\n");
    printf("| %-20s | %-12s | %-12s | %-12s |\n", "Algorithm", "Final Cost", "Evaluations", "Time");
    printf("|----------------------|--------------|--------------|--------------|\n");

    run_competition("2-Opt SIMD Local", solver_2opt_simd_run, &instance, &budget);

    return 0;
}
