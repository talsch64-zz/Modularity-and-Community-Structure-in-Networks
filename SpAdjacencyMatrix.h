
#ifndef PROJECT_SPADJACENCYMATRIX_H
#define PROJECT_SPADJACENCYMATRIX_H
#include "LinkedList.h"


/* SpAdjacencyMatrix:
 * Sparse matrix data structure for adjacency matrix.
 * Matrix[i,j] is 1 if there is an edge between vertex_i and vertex_j, else 0
 *
 * Members:
    * n: Matrix size (n*n)
    * rows
*/

/* ------------------------------------------------------------------------------------------------------------------*/

typedef struct SpAdjacencyMatrix {
    /* Matrix size (n*n) */
    int		n;
    LinkedList **rows;
} SpAdjacencyMatrix;


/* Allocates a new linked-lists sparse matrix of size n */
SpAdjacencyMatrix* allocateSpAdjMat(int n);

/* constructor for SpAdjacencyMatrix by adjacency matrix file */
SpAdjacencyMatrix* createAdjMatrixFromInput(char * filePath);

/* creates sub adjacency sparse-matrix of A, corresponding to the rows and columns in verticesGroup */
SpAdjacencyMatrix* createGroupSpAdjMatrix(SpAdjacencyMatrix *A, int *group, int groupSize);

/* getter for A[i,j]
 * returns 1 if there is an edge between the i's vertex and the j's vertex within the group (group[i], group[j)
 * otherwise, return 0 */
int getA(SpAdjacencyMatrix *A, int vertex1, int vertex2);

/* multiplies matrix A by vector v, into pre-allocated vector result
 * result = (A) x (vector) */
void multSpAdjMatByVector(SpAdjacencyMatrix *A, double *vector, double *result);

/* frees SpAdjacencyMatrix A */
void freeSpAdjMatrix(SpAdjacencyMatrix *A);





#endif
