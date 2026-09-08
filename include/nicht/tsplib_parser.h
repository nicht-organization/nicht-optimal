#ifndef NICHT_TSPLIB_PARSER_H
#define NICHT_TSPLIB_PARSER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define MAX_NODES 1024
#define MAX_TSPLIB_NODES MAX_NODES

typedef enum {
    NODE_COORD_EUC_2D,
    NODE_COORD_CEIL_2D,
    NODE_COORD_GEO,
    NODE_COORD_EXPLICIT
} TSPLIBCoordType;

typedef struct {
    float x;
    float y;
} TSPNode2D;

typedef struct {
    char name[64];
    uint32_t dimension;
    TSPLIBCoordType coord_type;
    TSPNode2D nodes[MAX_NODES];
    float weight_matrix[MAX_NODES * MAX_NODES]; /* 1D Tape Matrix */
} TSPInstance;

/* Compatibility typedef */
typedef TSPInstance TSPLIBInstance;

/* Zero-allocation parser for raw memory buffers containing .tsp files */
static inline bool tsplib_parse_buffer(const char *buffer, TSPInstance *out_inst) {
    if (!buffer || !out_inst) return false;
    memset(out_inst, 0, sizeof(TSPInstance));
    out_inst->coord_type = NODE_COORD_EUC_2D;

    const char *line = buffer;
    bool reading_coords = false;

    while (*line) {
        if (strncmp(line, "NAME", 4) == 0) {
            sscanf(line, "NAME : %63s", out_inst->name);
        } else if (strncmp(line, "DIMENSION", 9) == 0) {
            sscanf(line, "DIMENSION : %u", &out_inst->dimension);
            if (out_inst->dimension > MAX_NODES) return false;
        } else if (strncmp(line, "NODE_COORD_SECTION", 18) == 0) {
            reading_coords = true;
        } else if (strncmp(line, "EOF", 3) == 0) {
            break;
        } else if (reading_coords) {
            uint32_t id;
            float x, y;
            if (sscanf(line, "%u %f %f", &id, &x, &y) == 3) {
                if (id >= 1 && id <= out_inst->dimension) {
                    out_inst->nodes[id - 1].x = x;
                    out_inst->nodes[id - 1].y = y;
                }
            }
        }

        const char *next = strchr(line, '\n');
        if (!next) break;
        line = next + 1;
    }

    /* Compute explicit distance weight matrix */
    uint32_t n = out_inst->dimension;
    for (uint32_t i = 0; i < n; i++) {
        for (uint32_t j = 0; j < n; j++) {
            if (i == j) {
                out_inst->weight_matrix[i * n + j] = 0.0f;
            } else {
                float dx = out_inst->nodes[i].x - out_inst->nodes[j].x;
                float dy = out_inst->nodes[i].y - out_inst->nodes[j].y;
                out_inst->weight_matrix[i * n + j] = sqrtf(dx * dx + dy * dy);
            }
        }
    }

    return true;
}

#endif /* NICHT_TSPLIB_PARSER_H */
