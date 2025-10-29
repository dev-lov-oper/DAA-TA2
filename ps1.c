#include <stdio.h>
#include <limits.h>

#define MAX 1000
#define INF 1000000000

typedef struct {
    int x, y;
    int weight;
} Edge;

int main() {
    int n, m;
    printf("Enter number of vertices and edges: ");
    scanf("%d %d", &n, &m);

    Edge edges[m];
    printf("Enter each edge (u v w):\n");
    for (int i = 0; i < m; i++) {
        scanf("%d %d %d", &edges[i].x, &edges[i].y, &edges[i].weight);
        edges[i].x--; edges[i].y--; // 0-indexed
    }

    int source;
    printf("Enter the source vertex: ");
    scanf("%d", &source);
    source--;

    int distance[MAX], parent[MAX];
    for (int i = 0; i < n; i++) {
        distance[i] = INF;
        parent[i] = -1;
    }
    distance[source] = 0;

    // Relax edges n-1 times
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < m; j++) {
            int u = edges[j].x;
            int v = edges[j].y;
            int w = edges[j].weight;
            if (distance[u] != INF && distance[u] + w < distance[v]) {
                distance[v] = distance[u] + w;
                parent[v] = u;
            }
        }
    }

    // Check for negative cycle
    int x = -1;
    for (int j = 0; j < m; j++) {
        int u = edges[j].x;
        int v = edges[j].y;
        int w = edges[j].weight;
        if (distance[u] != INF && distance[u] + w < distance[v]) {
            x = v;
            parent[v] = u;
            break;
        }
    }

    if (x == -1) {
        printf("No negative cycle found.\n");
    } else {
        // Move back n steps to ensure we're inside the cycle
        for (int i = 0; i < n; i++)
            x = parent[x];

        // Reconstruct the cycle
        int cycle[MAX];
        int idx = 0;
        int start = x;

        cycle[idx++] = start; // first vertex
        int v = parent[start];
        while (v != start) {
            cycle[idx++] = v;
            v = parent[v];
        }
        cycle[idx++] = start; // add start again to close the cycle

        printf("Negative cycle: ");
        for (int i = idx - 1; i >= 0; i--) {
            printf("%d ", cycle[i] + 1); // convert to 1-indexed
        }
        printf("\n");
    }

    return 0;
}
