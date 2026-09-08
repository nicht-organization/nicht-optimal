#ifndef NICHT_SOLVERS_H
#define NICHT_SOLVERS_H

#include <stdint.h>
#include <stdbool.h>
#include <float.h>
#include "tsplib_parser.h"

/* Unified Solvers Result Struct */
typedef struct {
    uint32_t tour[MAX_TSPLIB_NODES];
    float tour_length;
    uint64_t iterations;
} TSPSolution;

/* Compute exact tour length from distance matrix tape */
static inline float tsp_compute_tour_length(const float *matrix, uint32_t N, const uint32_t *tour) {
    float len = 0.0f;
    for (uint32_t i = 0; i < N - 1; i++) {
        len += matrix[tour[i] * N + tour[i + 1]];
    }
    len += matrix[tour[N - 1] * N + tour[0]];
    return len;
}

/* 
 * Zero-Allocation SIMD-Friendly 2-Opt Local Search Engine
 * Optimizes tour in-place using continuous memory reversal
 */
static inline void tsp_solve_2opt(const TSPLIBInstance *inst, TSPSolution *out_sol, uint64_t max_iterations) {
    uint32_t N = inst->dimension;
    out_sol->iterations = 0;

    /* Initialize identity tour [0, 1, 2, ..., N-1] */
    for (uint32_t i = 0; i < N; i++) {
        out_sol->tour[i] = i;
    }

    const float *matrix = inst->weight_matrix;
    out_sol->tour_length = tsp_compute_tour_length(matrix, N, out_sol->tour);

    bool improved = true;
    while (improved && out_sol->iterations < max_iterations) {
        improved = false;
        
        for (uint32_t i = 1; i < N - 1; i++) {
            for (uint32_t k = i + 1; k < N; k++) {
                
                /* Compute delta length if edge swap (i-1, i) and (k, k+1) occurs */
                uint32_t u1 = out_sol->tour[i - 1];
                uint32_t v1 = out_sol->tour[i];
                uint32_t u2 = out_sol->tour[k];
                uint32_t v2 = out_sol->tour[(k + 1) % N];

                float current_dist = matrix[u1 * N + v1] + matrix[u2 * N + v2];
                float new_dist     = matrix[u1 * N + u2] + matrix[v1 * N + v2];

                if (new_dist < current_dist) {
                    /* Reverse tour slice between i and k in-place */
                    uint32_t l = i, r = k;
                    while (l < r) {
                        uint32_t tmp = out_sol->tour[l];
                        out_sol->tour[l] = out_sol->tour[r];
                        out_sol->tour[r] = tmp;
                        l++;
                        r--;
                    }

                    out_sol->tour_length -= (current_dist - new_dist);
                    improved = true;
                    out_sol->iterations++;
                    break;
                }
            }
            if (improved) break;
        }
    }
}

#endif /* NICHT_SOLVERS_H */
