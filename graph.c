#include "graph.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct graph {
    uint32_t vertices;
    bool directed;
    bool *visited;
    char **names;
    uint32_t **weights;
} Graph;

Graph *graph_create(uint32_t vertices, bool directed) {
    Graph *g = calloc(1, sizeof(Graph));
    if (g == NULL) {
        return NULL;
    }
    g->vertices = vertices;
    g->directed = directed;
    g->visited = calloc(vertices, sizeof(bool));
    g->names = calloc(vertices, sizeof(char *));
    g->weights = calloc(vertices, sizeof(g->weights[0]));
    for (uint32_t i = 0; i < vertices; ++i) {
        g->weights[i] = calloc(vertices, sizeof(g->weights[0][0]));
    }
    return g;
}

void graph_free(Graph **gp) {
    if (gp == NULL || *gp == NULL) {
        return;
    }
    Graph *g = *gp;
    for (uint32_t i = 0; i < g->vertices; ++i) {
        free(g->names[i]);
    }
    free(g->names);
    for (uint32_t i = 0; i < g->vertices; ++i) {
        free(g->weights[i]);
    }
    free(g->weights);
    free(g->visited);
    free(g);
    *gp = NULL;
}

uint32_t graph_vertices(const Graph *g) {
    if (g == NULL) {
        return 0;
    }
    return g->vertices;
}

void graph_add_vertex(Graph *g, const char *name, uint32_t v) {
    if (g == NULL || v >= g->vertices || name == NULL) {
        return;
    }
    if (g->names[v]) {
        free(g->names[v]);
    }
    g->names[v] = strdup(name);
}

const char *graph_get_vertex_name(const Graph *g, uint32_t v) {
    if (g == NULL || v >= g->vertices) {
        return NULL;
    }
    return g->names[v];
}

char **graph_get_names(const Graph *g) {
    if (g == NULL) {
        return NULL;
    }
    return g->names;
}

void graph_add_edge(Graph *g, uint32_t start, uint32_t end, uint32_t weight) {
    if (g == NULL || start >= g->vertices || end >= g->vertices) {
        return;
    }
    g->weights[start][end] = weight;
    if (!g->directed) {
        g->weights[end][start] = weight;
    }
}

uint32_t graph_get_weight(const Graph *g, uint32_t start, uint32_t end) {
    if (g == NULL || start >= g->vertices || end >= g->vertices) {
        return 0;
    }
    return g->weights[start][end];
}

void graph_visit_vertex(Graph *g, uint32_t v) {
    if (g == NULL || v >= g->vertices) {
        return;
    }
    g->visited[v] = true;
}

void graph_unvisit_vertex(Graph *g, uint32_t v) {
    if (g == NULL || v >= g->vertices) {
        return;
    }
    g->visited[v] = false;
}

bool graph_visited(const Graph *g, uint32_t v) {
    if (g == NULL || v >= g->vertices) {

        return false;
    }
    return g->visited[v];
}

void graph_print(const Graph *g) {
    if (g == NULL) {
        return;
    }
    printf("Graph with %u vertices:\n", g->vertices);

    printf("Vertex Names:\n");
    for (uint32_t i = 0; i < g->vertices; ++i) {
        printf("%u: %s\n", i, g->names[i]);
    }

    printf("Adjacency Matrix:\n");
    for (uint32_t i = 0; i < g->vertices; ++i) {
        for (uint32_t j = 0; j < g->vertices; ++j) {
            printf("%u ", g->weights[i][j]);
        }
        printf("\n");
    }

    printf("Visited Vertices:\n");
    for (uint32_t i = 0; i < g->vertices; ++i) {
        printf("%u: %s\n", i, g->visited[i] ? "Visited" : "Not Visited");
    }
}