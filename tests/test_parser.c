#include <stdio.h>
#include <assert.h>
#include "../include/nicht/tsplib_parser.h"

int main(void) {
    printf("=== Running TSPLIB Parser Unit Test ===\n");

    const char *sample_tsp = 
        "NAME : test_5\n"
        "TYPE : TSP\n"
        "DIMENSION : 3\n"
        "EDGE_WEIGHT_TYPE : EUC_2D\n"
        "NODE_COORD_SECTION\n"
        "1 0.0 0.0\n"
        "2 3.0 0.0\n"
        "3 0.0 4.0\n"
        "EOF\n";

    TSPLIBInstance inst;
    bool success = tsplib_parse_buffer(sample_tsp, &inst);
    assert(success);
    assert(inst.dimension == 3);

    /* Verify EUC_2D distances: (0,0)->(3,0) = 3.0, (0,0)->(0,4) = 4.0, (3,0)->(0,4) = 5.0 */
    printf("Node 1 -> Node 2 Dist: %.2f (Expected 3.00)\n", inst.weight_matrix[0 * 3 + 1]);
    printf("Node 1 -> Node 3 Dist: %.2f (Expected 4.00)\n", inst.weight_matrix[0 * 3 + 2]);
    printf("Node 2 -> Node 3 Dist: %.2f (Expected 5.00)\n", inst.weight_matrix[1 * 3 + 2]);

    assert(fabsf(inst.weight_matrix[0 * 3 + 1] - 3.0f) < 1e-4f);
    assert(fabsf(inst.weight_matrix[0 * 3 + 2] - 4.0f) < 1e-4f);
    assert(fabsf(inst.weight_matrix[1 * 3 + 2] - 5.0f) < 1e-4f);

    printf("=== TSPLIB Parser Unit Test Passed Successfully! ===\n");
    return 0;
}
