#ifndef NICHT_OPTIMAL_H
#define NICHT_OPTIMAL_H

/* Core TSPLIB Parser & Matrix Engine */
#include "tsplib_parser.h"

/* Core Solvers (2-Opt, local search, etc.) */
#include "solvers.h"

/* 
 * Modular Vendor Include Guards: 
 * Resolves nicht-collections primitives if submodule is present 
 */
#if __has_include(<nicht/collections.h>)
    #include <nicht/collections.h>
#elif __has_include("../../vendor/nicht-collections/include/nicht/collections.h")
    #include "../../vendor/nicht-collections/include/nicht/collections.h"
#endif

#endif /* NICHT_OPTIMAL_H */
