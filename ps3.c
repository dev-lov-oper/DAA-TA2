#include <stdio.h>
#include <float.h>

#define V 4        // Number of routers
#define INF DBL_MAX

// Structure to represent a directed edge
struct Edge {
    int u, v;
    int w;        //weight
};

// Function to perform Bellman-Ford+
void bellmanFord(struct Edge edges[], int E, int source) {
    double dist[V];
    for (int i = 0; i < V; i++)
        dist[i] = INF;
    dist[source] = 0;

    // Relax all edges V-1 times
    for (int i = 0; i < V - 1; i++) {
        int updated = 0;
        for (int j = 0; j < E; j++) {
            int u = edges[j].u, v = edges[j].v, w = edges[j].w;
            if (dist[u] != INF && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                updated = 1;
            }
        }
        if (!updated) break; // Optimization: stop early if no update
    }

    // Check for negative weight cycles
    for (int j = 0; j < E; j++) {
        int u = edges[j].u, v = edges[j].v, w = edges[j].w;
        if (dist[u] != INF && dist[u] + w < dist[v]) {
            printf("Error: Negative cycle detected. Shortest delays not well-defined.\n");
            return;
        }
    }

    // Router names
    const char* names[V] = {"A", "B", "C", "D"};

    // Print shortest delays from source
    printf("Shortest delay from A:\n");
    for (int i = 0; i < V; i++) {
        if (i != source)
            printf("%s = %.0f\n", names[i], dist[i]);
    }

    
    printf("\nPerformance note:\n");
    printf("Adjacency List  better for sparse networks (few links).\n");
    printf("Adjacency Matrix  better for dense networks (many connections).\n");
}

int main() {
    // Define edges of the network
    struct Edge edges[] = {
        {0, 1, 4},   // A -> B
        {0, 2, 5},   // A -> C
        {1, 2, -2},  // B -> C (fast lane)
        {2, 3, 3},   // C -> D
        {3, 1, 1}    // D -> B
    };
    int E = sizeof(edges)/sizeof(edges[0]);
    int source = 0; // Starting from router A

    bellmanFord(edges, E, source);

    return 0;
}

