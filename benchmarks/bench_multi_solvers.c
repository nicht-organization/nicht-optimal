#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include "../include/nicht/optimal.h"

/* Nearest Neighbor Constructive Heuristic */
static inline void tsp_solve_nearest_neighbor(const TSPLIBInstance *inst, TSPSolution *out_sol) {
    uint32_t N = inst->dimension;
    bool visited[MAX_TSPLIB_NODES] = {false};
    const float *matrix = inst->weight_matrix;

    out_sol->tour[0] = 0;
    visited[0] = true;

    for (uint32_t i = 0; i < N - 1; i++) {
        uint32_t curr = out_sol->tour[i];
        uint32_t best_next = 0;
        float min_dist = FLT_MAX;

        for (uint32_t j = 0; j < N; j++) {
            if (!visited[j]) {
                float dist = matrix[curr * N + j];
                if (dist < min_dist) {
                    min_dist = dist;
                    best_next = j;
                }
            }
        }
        out_sol->tour[i + 1] = best_next;
        visited[best_next] = true;
    }
    out_sol->tour_length = tsp_compute_tour_length(matrix, N, out_sol->tour);
    out_sol->iterations = N;
}

/* Hybrid Solver: NN Seed followed by 2-Opt */
static inline void tsp_solve_hybrid_nn_2opt(const TSPLIBInstance *inst, TSPSolution *out_sol, uint64_t max_iterations) {
    tsp_solve_nearest_neighbor(inst, out_sol);

    uint32_t N = inst->dimension;
    const float *matrix = inst->weight_matrix;
    bool improved = true;
    uint64_t iters = 0;

    while (improved && iters < max_iterations) {
        improved = false;
        for (uint32_t i = 1; i < N - 1; i++) {
            for (uint32_t k = i + 1; k < N; k++) {
                uint32_t u1 = out_sol->tour[i - 1];
                uint32_t v1 = out_sol->tour[i];
                uint32_t u2 = out_sol->tour[k];
                uint32_t v2 = out_sol->tour[(k + 1) % N];

                float current_dist = matrix[u1 * N + v1] + matrix[u2 * N + v2];
                float new_dist     = matrix[u1 * N + u2] + matrix[v1 * N + v2];

                if (new_dist < current_dist) {
                    uint32_t l = i, r = k;
                    while (l < r) {
                        uint32_t tmp = out_sol->tour[l];
                        out_sol->tour[l] = out_sol->tour[r];
                        out_sol->tour[r] = tmp;
                        l++; r--;
                    }
                    out_sol->tour_length -= (current_dist - new_dist);
                    improved = true;
                    iters++;
                    break;
                }
            }
            if (improved) break;
        }
    }
    out_sol->iterations += iters;
}

static double get_time_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec * 1e-6;
}

int main(void) {
    printf("=================================================================\n");
    printf("     nicht-optimal: BIG DATA MULTI-SOLVER BENCHMARK SUITE       \n");
    printf("=================================================================\n\n");

    uint32_t node_counts[] = {100, 300, 500};
    size_t num_runs = sizeof(node_counts) / sizeof(node_counts[0]);

    for (size_t r = 0; r < num_runs; r++) {
        uint32_t N = node_counts[r];
        TSPLIBInstance inst;
        inst.dimension = N;
        snprintf(inst.name, sizeof(inst.name), "synth_%u", N);

        srand(1337);
        for (uint32_t i = 0; i < N; i++) {
            inst.nodes[i].x = (float)(rand() % 10000) / 10.0f;
            inst.nodes[i].y = (float)(rand() % 10000) / 10.0f;
        }

        for (uint32_t i = 0; i < N; i++) {
            for (uint32_t j = 0; j < N; j++) {
                float dx = inst.nodes[i].x - inst.nodes[j].x;
                float dy = inst.nodes[i].y - inst.nodes[j].y;
                inst.weight_matrix[i * N + j] = sqrtf(dx * dx + dy * dy);
            }
        }

        printf("--- Benchmarking Graph Matrix: %u Nodes (%u Elements) ---\n", N, N * N);

        /* 1. Pure 2-Opt */
        TSPSolution sol_2opt;
        double t0 = get_time_ms();
        tsp_solve_2opt(&inst, &sol_2opt, 1000000);
        double t_2opt = get_time_ms() - t0;

        /* 2. Nearest Neighbor */
        TSPSolution sol_nn;
        t0 = get_time_ms();
        tsp_solve_nearest_neighbor(&inst, &sol_nn);
        double t_nn = get_time_ms() - t0;

        /* 3. Hybrid NN + 2-Opt */
        TSPSolution sol_hybrid;
        t0 = get_time_ms();
        tsp_solve_hybrid_nn_2opt(&inst, &sol_hybrid, 1000000);
        double t_hybrid = get_time_ms() - t0;

        printf("  [Pure 2-Opt]     Length: %8.2f | Iters: %5llu | Time: %6.3f ms\n",
               sol_2opt.tour_length, (unsigned long long)sol_2opt.iterations, t_2opt);
        printf("  [Nearest Neigh]  Length: %8.2f | Iters: %5llu | Time: %6.3f ms\n",
               sol_nn.tour_length, (unsigned long long)sol_nn.iterations, t_nn);
        printf("  [Hybrid NN+2Opt] Length: %8.2f | Iters: %5llu | Time: %6.3f ms\n\n",
               sol_hybrid.tour_length, (unsigned long long)sol_hybrid.iterations, t_hybrid);
    }

    printf("=================================================================\n");
    return 0;
}
