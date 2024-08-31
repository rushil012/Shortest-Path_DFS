#include "path.h"

#include "stack.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct path {
    uint32_t total_weight;
    Stack *vertices;
} Path;

Path *path_create(uint32_t capacity) {
    Path *path = (Path *) malloc(sizeof(Path));
    if (!path) {
        return NULL;
    }

    path->total_weight = 0;
    path->vertices = stack_create(capacity);
    if (!path->vertices) {
        free(path);
        return NULL;
    }

    return path;
}

void path_free(Path **pp) {
    if (pp && *pp) {
        stack_free(&((*pp)->vertices));
        free(*pp);
        *pp = NULL;
    }
}

uint32_t path_vertices(const Path *p) {
    if (p) {
        return stack_size(p->vertices);
    }
    return 0;
}

uint32_t path_distance(const Path *p) {
    if (p) {
        return p->total_weight;
    }
    return 0;
}

void path_add(Path *p, uint32_t val, const Graph *g) {
    if (p && g) {
        if (stack_empty(p->vertices)) {
            stack_push(p->vertices, val);
        } else {
            uint32_t last_vertex;
            stack_peek(p->vertices, &last_vertex);
            uint32_t distance = graph_get_weight(g, last_vertex, val);
            stack_push(p->vertices, val);
            p->total_weight += distance;
        }
    }
}

uint32_t path_remove(Path *p, const Graph *g) {
    uint32_t removed_vertex = 0; 

    if (p && g && !stack_empty(p->vertices)) {
        stack_pop(p->vertices, &removed_vertex);

        if (!stack_empty(p->vertices)) {
            uint32_t last_vertex;
            stack_peek(p->vertices, &last_vertex);
            uint32_t distance = graph_get_weight(g, last_vertex, removed_vertex);
            p->total_weight -= distance;
        } else {
            p->total_weight = 0;
        }
    }

    return removed_vertex;
}

void path_copy(Path *dst, const Path *src) {
    if (dst && src) {
        stack_copy(dst->vertices, src->vertices);
        dst->total_weight = src->total_weight;
    }
}

void path_print(const Path *p, FILE *outfile, const Graph *g) {
    if (p && outfile && g) {
        fprintf(outfile, "Alissa starts at:\n");

        Stack *temp_stack = stack_create(stack_size(p->vertices));
        while (!stack_empty(p->vertices)) {
            uint32_t vertex;
            stack_pop(p->vertices, &vertex);
            stack_push(temp_stack, vertex);
        }
        while (!stack_empty(temp_stack)) {
            uint32_t vertex;
            stack_pop(temp_stack, &vertex);
            fprintf(outfile, "%s\n", graph_get_vertex_name(g, vertex));
        }
        // fprintf(outfile, "%s\n", graph_get_vertex_name(g, 0));
        fprintf(outfile, "Total Distance: %u\n", p->total_weight);
        stack_free(&temp_stack);
    }
}