#include <iostream>     // For input and output
#include <vector>       // For vector data structure
#include <queue>        // For queue used in BFS
#include <stack>        // For stack used in DFS
#include <omp.h>        // OpenMP library for parallel programming
#include <time.h>       // For measuring execution time

using namespace std;


// ====================== BFS ======================

// Sequential BFS Function
void bfsSequential(vector<vector<int>>& graph, int start, int n) {

    // Array to track visited nodes
    vector<bool> visited(n, false);

    // Queue used in BFS traversal
    queue<int> q;

    // Push starting node into queue
    q.push(start);

    // Mark start node as visited
    visited[start] = true;

    cout << "Sequential BFS Traversal : ";

    // Continue until queue becomes empty
    while(!q.empty()) {

        // Get front node from queue
        int node = q.front();

        // Remove node from queue
        q.pop();

        // Print current node
        cout << node << " ";

        // Visit all neighbors of current node
        for(auto neighbor : graph[node]) {

            // If neighbor is not visited
            if(!visited[neighbor]) {

                // Mark neighbor visited
                visited[neighbor] = true;

                // Push neighbor into queue
                q.push(neighbor);
            }
        }
    }

    cout << endl;
}



// ====================== Parallel BFS ======================

void bfsParallel(vector<vector<int>>& graph, int start, int n) {

    // Visited array
    vector<bool> visited(n, false);

    // Queue for BFS
    queue<int> q;

    // Insert starting node
    q.push(start);

    // Mark as visited
    visited[start] = true;

    cout << "Parallel BFS Traversal : ";

    while(!q.empty()) {

        // Get front node
        int node = q.front();

        // Remove from queue
        q.pop();

        // Print node
        cout << node << " ";

        // Parallel loop using OpenMP
        #pragma omp parallel for
        for(int i = 0; i < graph[node].size(); i++) {

            int neighbor = graph[node][i];

            // Critical section prevents race condition
            #pragma omp critical
            {
                // If neighbor not visited
                if(!visited[neighbor]) {

                    // Mark visited
                    visited[neighbor] = true;

                    // Push into queue
                    q.push(neighbor);
                }
            }
        }
    }

    cout << endl;
}



// ====================== DFS ======================

// Sequential DFS Function
void dfsSequential(vector<vector<int>>& graph, int start, int n) {

    // Visited array
    vector<bool> visited(n, false);

    // Stack used in DFS
    stack<int> s;

    // Push starting node
    s.push(start);

    // Mark visited
    visited[start] = true;

    cout << "Sequential DFS Traversal : ";

    // Continue until stack is empty
    while(!s.empty()) {

        // Get top node
        int node = s.top();

        // Remove node from stack
        s.pop();

        // Print node
        cout << node << " ";

        // Visit all neighbors
        for(auto neighbor : graph[node]) {

            // If not visited
            if(!visited[neighbor]) {

                // Mark visited
                visited[neighbor] = true;

                // Push into stack
                s.push(neighbor);
            }
        }
    }

    cout << endl;
}



// ====================== Parallel DFS ======================

void dfsParallel(vector<vector<int>>& graph, int start, int n) {

    // Visited array
    vector<bool> visited(n, false);

    // Stack for DFS
    stack<int> s;

    // Push start node
    s.push(start);

    // Mark visited
    visited[start] = true;

    cout << "Parallel DFS Traversal : ";

    while(!s.empty()) {

        // Get top node
        int node = s.top();

        // Remove top node
        s.pop();

        // Print node
        cout << node << " ";

        // Parallel loop
        #pragma omp parallel for
        for(int i = 0; i < graph[node].size(); i++) {

            int neighbor = graph[node][i];

            // Critical section
            #pragma omp critical
            {
                // If not visited
                if(!visited[neighbor]) {

                    // Mark visited
                    visited[neighbor] = true;

                    // Push into stack
                    s.push(neighbor);
                }
            }
        }
    }

    cout << endl;
}



int main() {

    // ================= RANDOM GRAPH =================

    int n = 500;         // Number of nodes
    int edges = 2000;   // Number of edges

    // Adjacency list representation
    vector<vector<int>> graph(n);

    // Generate random graph
    for(int i = 0; i < edges; i++) {

        int u = rand() % n;
        int v = rand() % n;

        // Undirected graph
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    // Starting node
    int start = 0;



    /*
    ================= MANUAL INPUT =================

    int n, edges;

    cout << "Enter number of nodes: ";
    cin >> n;

    vector<vector<int>> graph(n);

    cout << "Enter number of edges: ";
    cin >> edges;

    cout << "Enter edges (u v): ";

    for(int i = 0; i < edges; i++) {

        int u, v;
        cin >> u >> v;

        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    int start;

    cout << "Enter start node: ";
    cin >> start;

    */


    // Variables to store execution time
    clock_t start_time, end_time;



    // ================= BFS =================

    // Start timer
    start_time = clock();

    // Call sequential BFS
    bfsSequential(graph, start, n);

    // Stop timer
    end_time = clock();

    // Calculate time in milliseconds
    double bfs_seq =
    ((double)(end_time - start_time) * 1000)
    / CLOCKS_PER_SEC;

    cout << "Sequential BFS Time : "
         << bfs_seq << " ms\n";


    // Parallel BFS timing
    start_time = clock();

    bfsParallel(graph, start, n);

    end_time = clock();

    double bfs_par =
    ((double)(end_time - start_time) * 1000)
    / CLOCKS_PER_SEC;

    cout << "Parallel BFS Time : "
         << bfs_par << " ms\n";

    // Speedup calculation
    cout << "BFS Speedup : "
         << bfs_seq / bfs_par << "\n\n";



    // ================= DFS =================

    start_time = clock();

    dfsSequential(graph, start, n);

    end_time = clock();

    double dfs_seq =
    ((double)(end_time - start_time) * 1000)
    / CLOCKS_PER_SEC;

    cout << "Sequential DFS Time : "
         << dfs_seq << " ms\n";


    // Parallel DFS timing
    start_time = clock();

    dfsParallel(graph, start, n);

    end_time = clock();

    double dfs_par =
    ((double)(end_time - start_time) * 1000)
    / CLOCKS_PER_SEC;

    cout << "Parallel DFS Time : "
         << dfs_par << " ms\n";

    // Speedup calculation
    cout << "DFS Speedup : "
         << dfs_seq / dfs_par << endl;

    return 0;
}