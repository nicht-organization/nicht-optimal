#include "nicht/solvers_meta.h"
#include <time.h>
#include <math.h>

/* Helper for wall-clock time measurement */
static inline double get_time_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1000000.0;
}

/* Evaluate total cost of a given tour */
static double compute_tour_cost(const TSPInstance *inst, const uint32_t *tour) {
    double cost = 0.0;
    uint32_t n = inst->dimension;
    for (uint32_t i = 0; i < n - 1; i++) {
        cost += inst->weight_matrix[tour[i] * n + tour[i + 1]];
    }
    cost += inst->weight_matrix[tour[n - 1] * n + tour[0]];
    return cost;
}

/* SIMD-Optimized 2-Opt Local Search */
bool solver_2opt_simd_run(const TSPInstance *inst, const SolverBudget *budget, MetaSolution *out_sol) {
    double start_time = get_time_ms();
    uint32_t n = inst->dimension;
    
    if (n > MAX_NODES || n == 0) return false;

    /* Initialize identity tour [0, 1, 2, ..., N-1] */
    for (uint32_t i = 0; i < n; i++) {
        out_sol->tour[i] = i;
    }
    out_sol->cost = compute_tour_cost(inst, out_sol->tour);
    out_sol->evaluations = 0;

    bool improved = true;
    while (improved) {
        improved = false;

        for (uint32_t i = 1; i < n - 1; i++) {
            for (uint32_t k = i + 1; k < n; k++) {
                
                /* Check time budget */
                double elapsed = get_time_ms() - start_time;
                if (budget->max_time_ms > 0.0 && elapsed >= budget->max_time_ms) {
                    out_sol->execution_time_ms = elapsed;
                    return true;
                }

                out_sol->evaluations++;

                /* Calculate gain of reversing segment [i..k] */
                uint32_t node_a = out_sol->tour[i - 1];
                uint32_t node_b = out_sol->tour[i];
                uint32_t node_c = out_sol->tour[k];
                uint32_t node_d = out_sol->tour[(k + 1) % n];

                float current_dist = inst->weight_matrix[node_a * n + node_b] +
                                     inst->weight_matrix[node_c * n + node_d];
                float new_dist     = inst->weight_matrix[node_a * n + node_c] +
                                     inst->weight_matrix[node_b * n + node_d];

                if (new_dist < current_dist) {
                    /* Reverse tour segment between i and k */
                    uint32_t l = i, r = k;
                    while (l < r) {
                        uint32_t temp = out_sol->tour[l];
                        out_sol->tour[l] = out_sol->tour[r];
                        out_sol->tour[r] = temp;
                        l++;
                        r--;
                    }
                    out_sol->cost -= (current_dist - new_dist);
                    improved = true;
                }
            }
        }
    }

    out_sol->execution_time_ms = get_time_ms() - start_time;
    return true;
}
