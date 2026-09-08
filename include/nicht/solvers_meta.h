#ifndef NICHT_SOLVERS_META_H
#define NICHT_SOLVERS_META_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "tsplib_parser.h"

/* Standardized Meta-Heuristic Solution State */
typedef struct {
    uint32_t tour[MAX_NODES];       /* Node sequence array */
    double cost;                    /* Total tour length / objective cost */
    uint64_t evaluations;           /* Total candidate moves evaluated */
    double execution_time_ms;       /* Total elapsed search time */
} MetaSolution;

/* Standardized Execution Budget Constraints for Competitions */
typedef struct {
    double max_time_ms;             /* Maximum allowable wall-clock time */
    uint64_t max_evaluations;       /* Max move evaluations permitted */
    double target_cost;             /* Target cost threshold (early stop) */
} SolverBudget;

/* Competition Framework Solver Interface Function Pointer */
typedef bool (*MetaSolverFunc)(
    const TSPInstance *inst, 
    const SolverBudget *budget, 
    MetaSolution *out_sol
);

typedef enum {
    SOLVER_2OPT_SIMD,          /* SIMD Trajectory Local Search */
    SOLVER_SIMULATED_ANNEALING,/* Thermodynamic Trajectory */
    SOLVER_ANT_COLONY,         /* Swarm Intelligence */
    SOLVER_GENETIC_MEMETIC,    /* Population Evolutionary */
    SOLVER_APOPHATIC_SIEVE,    /* Subtractive Spectral B0 Filter */
    SOLVER_LKH_BRIDGE          /* State-of-the-Art Hyper-Heuristic */
} MetaSolverType;

/* Uniform Function Call for Competition Races */
bool solver_run_by_type(
    MetaSolverType type,
    const TSPInstance *inst,
    const SolverBudget *budget,
    MetaSolution *out_sol
);
/* 
 * Meta-Heuristic Solver Implementations 
 */

/* SIMD 2-Opt Local Search (Dense Matrix Cache Line Optimized) */
bool solver_2opt_simd_run(const TSPInstance *inst, const SolverBudget *budget, MetaSolution *out_sol);

/* Lin-Kernighan-Helsgaun (LKH-3 style) Variable K-Opt Hyper-Heuristic */
bool solver_lkh_bridge_run(const TSPInstance *inst, const SolverBudget *budget, MetaSolution *out_sol);

/* Apophatic Subtractive Sieve: Prunes non-promising edges via B0 friction bounds */
bool solver_apophatic_sieve_run(const TSPInstance *inst, const SolverBudget *budget, MetaSolution *out_sol);

#endif /* NICHT_SOLVERS_META_H */
