#include "graph.h"
#include "path.h"
#include "stack.h"
#include "vertices.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
void dfs(Graph *g, uint32_t start_vertex, Path *best_path, Path *temp_path) {
    if (g == NULL || start_vertex >= graph_vertices(g)) {
        return;
    }
    graph_visit_vertex(g, start_vertex);
    path_add(temp_path, start_vertex, g);

    if (path_vertices(temp_path) == graph_vertices(g)) {
        if (graph_get_weight(g, start_vertex, 0) > 0) {
            path_add(temp_path, 0, g);

            if (path_distance(temp_path) < path_distance(best_path)
                || path_distance(best_path) == 0) {

                path_copy(best_path, temp_path);
            }
            path_remove(temp_path, g);

        } else {
            bool hasPathBack = false;
            for (uint32_t i = 0; i < graph_vertices(g); ++i) {
                if (graph_get_weight(g, start_vertex, i) > 0 && graph_visited(g, i)) {
                    hasPathBack = true;
                    break;
                }
            }
            if (!hasPathBack) {
                printf("No path found! Alissa is lost!\n");
                exit(0);
            }
        }
    } else {
        for (uint32_t i = 0; i < graph_vertices(g); ++i) {
            if (graph_get_weight(g, start_vertex, i) > 0 && !graph_visited(g, i)) {
                dfs(g, i, best_path, temp_path);
            }
        }
    }
    graph_unvisit_vertex(g, start_vertex);
    path_remove(temp_path, g);
}

int main(int argc, char *argv[]) {
    FILE *infile = stdin;
    FILE *outfile = stdout;
    bool directed = false;
    char *input_filename = NULL;
    char *output_filename = NULL;
    int opt;
    while ((opt = getopt(argc, argv, "i:o:dh")) != -1) {
        switch (opt) {
        case 'i':
            input_filename = optarg;
            infile = fopen(input_filename, "r");
            if (infile == NULL) {
                fprintf(stderr, "Unable to open input file: %s\n", input_filename);
                return 1;
            }

            break;
        case 'o':
            output_filename = optarg;
            outfile = fopen(output_filename, "w");
            if (outfile == NULL) {
                fprintf(stderr, "Unable to open output file: %s\n", output_filename);
                return 1;
            }
            break;
        case 'd': directed = true; break;
        case 'h':
            printf("Usage: tsp [options]\n\n"
                   "-i infile    Specify the input file path containing the cities and edges\n"
                   "             of a graph. If not specified, the default input should be\n"
                   "             set as stdin.\n\n"
                   "-o outfile   Specify the output file path to print to. If not specified,\n"
                   "             the default output should be set as stdout.\n\n"
                   "-d           Specifies the graph to be directed.\n\n"
                   "-h           Prints out a help message describing the purpose of the\n"
                   "             graph and the command-line options it accepts, exiting the\n"
                   "             program afterwards.\n");
            return 0;
        default:
            fprintf(stderr,
                "Usage: tsp [options]\n\n"
                "-i infile    Specify the input file path containing the cities and edges\n"
                "             of a graph. If not specified, the default input should be\n"
                "             set as stdin.\n\n"
                "-o outfile   Specify the output file path to print to. If not specified,\n"
                "             the default output should be set as stdout.\n\n"
                "-d           Specifies the graph to be directed.\n\n"
                "-h           Prints out a help message describing the purpose of the\n"
                "             graph and the command-line options it accepts, exiting the\n"
                "             program afterwards.\n");
            return 1;
        }
    }
    uint32_t num_vertices;
    fscanf(infile, "%u\n", &num_vertices);
    Graph *g = graph_create(num_vertices, directed);
    for (uint32_t i = 0; i < num_vertices; ++i) {
        char name[100];
        fgets(name, sizeof(name), infile);
        name[strcspn(name, "\n")] = 0;

        graph_add_vertex(g, name, i);
    }
    uint32_t num_edges;
    fscanf(infile, "%u\n", &num_edges);

    for (uint32_t i = 0; i < num_edges; ++i) {
        uint32_t start, end, weight;
        fscanf(infile, "%u %u %u", &start, &end, &weight);
        graph_add_edge(g, start, end, weight);
    }

    Path *best_path = path_create(graph_vertices(g) + 1);
    Path *temp_path = path_create(graph_vertices(g) + 1);
    dfs(g, START_VERTEX, best_path, temp_path);
    path_print(best_path, outfile, g);

    path_free(&best_path);
    path_free(&temp_path);
    graph_free(&g);
    fclose(infile);
    fclose(outfile);
    return 0;
}