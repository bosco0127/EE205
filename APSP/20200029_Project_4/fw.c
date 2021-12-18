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
void PrintPath(int source, int dest, int** B) {
    if (source == dest) {
        printf(" %d",B[source-1][dest-1]);
        return;
    }
    
    // print i->k
    if (dest != B[source-1][dest-1]) {
        PrintPath(source, B[source-1][dest-1], B);
    }
    /* // print k
    printf(" %d",B[source-1][dest-1]);*/
    // print k->j
    PrintPath(B[source-1][dest-1], dest, B);
}

// Print the result
void PrintResult(int V, int** A, int** B) {
    // loop index
    int i,j;
    
    // print the result
    for(i=0; i < V; i++) {
        for(j=0; j < V; j++) {
            if(j != i) {
                if(A[i][j] < INF) {
                    printf("%d %d length: %d path: %d",i+1,j+1,A[i][j],i+1);
                    PrintPath(i+1,j+1,B);
                    printf("\n");
                }
                else {
                    printf("%d %d length: inf path: none\n",i+1,j+1);
                }
            }
        }
    }

    // return
    return;
}

// Floyd-Warshall algorithm
// needs graph.
void FloydWarshall(Graph_T graph) {
    // number of V & E
    int V = graph->V;
    int E = graph->E;
    // for vertex
    int v,w;
    // for weight
    int weight;
    // loop index
    int k,i,j;

    // 2D length array A
    int** A = (int **) malloc(V*sizeof(int*));
    // for malloc failure
    if (A == NULL) {
      fprintf(stderr, "Memory allocation failed @ array A\n");
      return;
    }
    // 2D internal vertex array B
    int** B = (int **) malloc(V*sizeof(int*));
    // for malloc failure
    if (B == NULL) {
      fprintf(stderr, "Memory allocation failed @ array B\n");
      return;
    }
    // allocate memory to A[i], B[i] and initialize.
    for(i=0; i < V; i++) {
        A[i] = (int *) malloc(V*sizeof(int));
        // for malloc failure
        if (A[i] == NULL) {
            fprintf(stderr, "Memory allocation failed @ array A[%d]\n",i);
            for(j=0;j<i;j++) free(A[i]);
            free(A);
            for(j=0;j<i;j++) free(B[i]);
            free(B);
            return;
        }        
        B[i] = (int *) malloc(V*sizeof(int));
        // for malloc failure
        if (B[i] == NULL) {
            fprintf(stderr, "Memory allocation failed @ array B[%d]\n",i);
            for(j=0;j<=i;j++) free(A[i]);
            free(A);
            for(j=0;j<i;j++) free(B[i]);
            free(B);
            return;
        }
        // set A[i] to 2*INF
        for(j=0; j < V; j++){
            // set A[i] to 2*INF
            A[i][j] = 2*INF;
            // set B[i] to zero
            B[i][j] = 0;
        }
    }    

    // initialize
    // in case of i=j
    for(i=0; i < V; i++) {
        // length
        A[i][i] = 0;
        // max label of internal node
        B[i][i] = i+1;
    }
    // in case of (i,j) is in E.
    for(j=0; j < E; j++) {
        // assignment
        v = graph->edge[j].dest;
        w = graph->edge[j].src;
        weight = graph->edge[j].weight;

        // A[i][j] = Lij
        if(weight < A[w-1][v-1]){
            A[w-1][v-1] = weight;
            // B[i][j] = i
            B[w-1][v-1] = v;
        }
    }

    // Optimal Structure
    for(k=0; k < V; k++){
        for(i=0; i < V; i++) {
            for(j=0; j < V; j++) {
                // min-length select
                if(A[i][k] + A[k][j] < A[i][j]) {
                    // length update
                    A[i][j] = A[i][k] + A[k][j];
                    // internal vertex update
                    B[i][j] = k+1;                               
                }
            }
        }
    }

    // Negative cycle detect
    for(i=0; i < V; i++) {
        // negative cycle detection
        if (A[i][i] < 0) {
            printf("Error: negative-cost cycle is found\n");
            return;
        }
    }

    // Print the result
    PrintResult(V, A, B);

    // free A & B
    for(i=0; i < V; i++) {
        free(A[i]);
        free(B[i]);
    }
    free(A);
    free(B);

    // return
    return;
}

int main(int argc, char* argv[]) {
    // print error message and stop program if the form is wrong
    if(argc >= 2){
        printf("Error: type in form of \"./fw < file\"\n");
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

    // do Floyd-Warshall
    FloydWarshall(graph);

    // Destroy graph
    DestroyGraph(graph);

    return 0;
}