#include <stdio.h>
#include <math.h>
#include <float.h>

#define MAX 20
#define INF DBL_MAX
#define EPS 1e-12  // For floating-point precision tolerance

// Structure for directed edge
typedef struct {
    int u, v;
    double w; // Weight = -log(rate)
} Edge;

// Currency names
const char* names[] = {"USD", "EUR", "GBP"};

int main() {
    int V = 3; // Number of currencies

    // Exchange rates table (0 means no direct conversion)
    double rate[3][3] = {
        {1, 0.9, 0},
        {0, 1, 0.8},
        {1.4, 0, 1}
    };

    // Build edge list
    Edge edges[MAX * MAX];
    int E = 0;
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (rate[i][j] > 0 && i != j) {
                edges[E++] = (Edge){i, j, -log(rate[i][j])};
            }
        }
    }

    // Bellman–Ford initialization
    double dist[MAX];
    int parent[MAX];
    for (int i = 0; i < V; i++) {
        dist[i] = INF;
        parent[i] = -1;
    }
    dist[0] = 0; // Start from USD (index 0)

    // Relax edges (V-1) times
    for (int i = 0; i < V - 1; i++) {
        int updated = 0;
        for (int j = 0; j < E; j++) {
            int u = edges[j].u, v = edges[j].v;
            double w = edges[j].w;
            if (dist[u] + w < dist[v] - EPS) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                updated = 1;
            }
        }
        if (!updated) break;
    }

    // Check for negative cycle (arbitrage)
    int cycle_start = -1;
    for (int j = 0; j < E; j++) {
        int u = edges[j].u, v = edges[j].v;
        double w = edges[j].w;
        if (dist[u] + w < dist[v] - EPS) {
            cycle_start = v;
            break;
        }
    }

    if (cycle_start == -1) {
        printf("No arbitrage opportunity detected.\n");
    } else {
        // Ensure cycle_start is inside the cycle
        for (int i = 0; i < V; i++)
            cycle_start = parent[cycle_start];

        // Reconstruct the cycle
        int cycle[MAX], k = 0, x = cycle_start;
        do {
            cycle[k++] = x;
            x = parent[x];
        } while (x != cycle_start && k < MAX);
        cycle[k++] = cycle_start;

        // Print the arbitrage cycle
        printf("Arbitrage opportunity detected:\n");
        for (int i = k - 1; i >= 0; i--) {
            printf("%s", names[cycle[i]]);
            if (i != 0) printf(" -> ");
        }

        // Compute profit (starting with $1)
        double amount = 1.0;
        for (int i = k - 1; i > 0; i--) {
            int from = cycle[i], to = cycle[i - 1];
            amount *= rate[from][to];
        }
        printf("\nProfit: $1 → $%.4f\n", amount);
    }

    return 0;
}
