#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define INF 100000000

typedef struct Graph* Graph_T;

/* Structure of Edge */
// contains src(source), dest(destination), and weight
struct Edge {
  int src;                // source vertex
  int dest;              // destination vertex
  int weight;   // weight of edge
};

/* Structure of Graph */
// contains number of vertices and edges and Edge type pointers for edge array.
struct Graph {
  int V;                // number of vertex
  int E;              // number of edge
  struct Edge* edge;   // pointer of edge array
};

// creates Graph structure
// needs int type V & E
Graph_T CreateGraph(int V, int E) {
    // Allocate memory for graph
    Graph_T graph;
    graph = (Graph_T) malloc(1*sizeof(struct Graph));
    // for malloc failure
    if (graph == NULL) {
      fprintf(stderr, "Can't allocate a memory for Graph_T\n");
      return NULL;
    }

    // assign V & E
    graph->V = V;
    graph->E = E;
    // allocate memory for edge array
    graph->edge = (struct Edge *)malloc(graph->E*sizeof(struct Edge));
    // for malloc failure
    if (graph->edge == NULL) {
        fprintf(stderr, "Can't allocate a memory for edge array of size %d\n", graph->E);   
        free(graph);
        return NULL;
    }

    // return graph
    return graph;
}

// free memory of graph
// needs Graph_T graph
void DestroyGraph(Graph_T graph){
    // if graph points nothing, do nothing.
    if(graph == NULL) return;

    // free edge
    free(graph->edge);
    // free graph itself
    free(graph);

    return;
}

// PrintPath by tracing inversely
void PrintPath(int source, int dest, int* B) {
    if (source == dest) {
        printf(" %d",dest);
        return;
    }
    
    // print last pointing path
    PrintPath(source, B[dest-1], B);
    // print current vertex
    printf(" %d",dest);
}

// Print the result
void PrintResult(int source, int V, int* A, int* B) {
    // loop index
    int i;
    
    // print the result
    for(i=0; i < V; i++) {
        if(i != source-1) {
            if(A[i] < INF) {
                printf("%d %d length: %d path:",source,i+1,A[i]);
                PrintPath(source, i+1, B);
                printf("\n");
            }
            else {
                printf("%d %d length: inf path: none\n",source,i+1);
            }
        }
    }

    // return
    return;
}

// Bellman-Ford algorithm
// needs graph, source.
// return 1 if negative cycle found else return 0.
int BellmanFord(Graph_T graph, int source) {
    // number of V & E
    int V = graph->V;
    int E = graph->E;
    // for vertex
    int v,w;
    // for weight
    int weight;
    // loop index
    int i,j;

    // length array A
    int* A = (int *) malloc(V*sizeof(int));
    // for malloc failure
    if (A == NULL) {
      fprintf(stderr, "Memory allocation failed @ array A\n");
      return 1;
    }
    // last pointing vertex array B
    int* B = (int *) malloc(V*sizeof(int));
    // for malloc failure
    if (B == NULL) {
      fprintf(stderr, "Memory allocation failed @ array B\n");
      return 1;
    }

    // initialize
    for(i=0; i < V; i++) {
        // length: set to INF
        A[i] = 2*INF;
        // last vertex: set to null
        B[i] = 0;
    }
    A[source-1] = 0;

    // Optimal Structure
    for(i=1; i <= V-1; i++) {
        for(j=0; j < E; j++) {
            // assignment
            v = graph->edge[j].dest;
            w = graph->edge[j].src;
            weight = graph->edge[j].weight;

            // min-length select
            if(A[w-1] + weight < A[v-1]) {
                // length update
                A[v-1] = A[w-1] + weight;
                // last pointing vertex update
                B[v-1] = w;                               
            }
        }
    }

    // Negative cycle detect
    for(j=0; j < E; j++) {
        // assignment
        v = graph->edge[j].dest;
        w = graph->edge[j].src;
        weight = graph->edge[j].weight;

        // negative cycle detection
        if (A[w-1] + weight < A[v-1]) {
            printf("Error: negative-cost cycle is found\n");
            return 1;
        }
    }

    // Print the result
    PrintResult(source, V, A, B);

    // free A & B
    free(A);
    free(B);

    // return
    return 0;
}

int main(int argc, char* argv[]) {
    // print error message and stop program if the form is wrong
    if(argc >= 3){
        printf("Error: type in form of \"./bf (natural number) < file\"\n");
        return 0;
    }

    // number of vertex & edge
    int V;
    int E;
    // source
    int source;
    // detect negative cycle
    int IsNegCyc = 0; // initailized as 0

    // scan V and E
    scanf("%d %d",&V,&E);

    // create graph
    Graph_T graph = CreateGraph(V,E);
    if(graph == NULL) { // If it is failed to creating DB,
        fprintf(stderr, "Failed to create graph\n"); // print error message
        assert(0); // assert program
    }

    // assign graph edge
    for(int i=0; i < E; i++){
        scanf("%d %d %d",&graph->edge[i].src,&graph->edge[i].dest,&graph->edge[i].weight);
    }

    // do Bellman-Ford
    if (argc == 1) {
        for(source = 1; source <= V; source++) {
            IsNegCyc = BellmanFord(graph,source);
            // break if negative cycle exists.
            if (IsNegCyc) break;
        }
    }
    else {
        // print error message and stop program if the form is wrong
        if(atoi(argv[1])<=0 || atoi(argv[1]) > V){
            printf("Error: type in form of \"./bf (vertex number) < file\"\n");
            return 0;
        }
        source = atoi(argv[1]);
        IsNegCyc = BellmanFord(graph,source);
    }
    

    // Destroy graph
    DestroyGraph(graph);

    return 0;
}