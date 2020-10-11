#ifndef PROJECT_MODULARITYMGROUPATRIX_H
#define PROJECT_MODULARITYMGROUPATRIX_H

#include "SpAdjacencyMatrix.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "Utils.h"

/*
 * ModularityGroupMatrix
 * Data structure that represents a container of a B[g]Hat info
 *
 * Members:
    * groupSize
    * verticesGroup: The vertices of the group g
    * adjMatrix: Adjacency matrix represented as sparse matrix
    * degreesVector: The degrees of the vertices in group g
    * M: Degrees sum of the original graph
    * f: fg according to the given instructions
    * norm: The Matrix's norm1
 */

/*------------------------------------------------------------------------------------------------------------------*/

typedef struct ModularityGroupMatrix {
    int groupSize;
    int *verticesGroup;
    SpAdjacencyMatrix *adjMatrix;
    double * degreesVector;
    int M;
    double *f;
    double norm;

} ModularityGroupMatrix;

/* constructor for the initial ModularityGroupMatrix */
ModularityGroupMatrix* initFirstModularityMatrix(SpAdjacencyMatrix *adjMatrix, int *verticesGroup, int groupSize);

/* initialize sub-ModularityGroupMatrix from an existing ModularityGroupMatrix */
ModularityGroupMatrix* initSubModularityGroupMatrix(ModularityGroupMatrix *modMatrix, int *verticesGroup, int groupSize);

/* assign result with the product of modMatrix Matrix and vector.
 * if withF == 1 then the computation includes fg, else not
 * if shift == 1 then the computation includes shifting by the matrix norm, else not */
void multModMatrixByVector(ModularityGroupMatrix *modMatrix, double *vector, double *result, bool withF, bool shift);

/* compute modularity of division
 * modularity = (divisionVector_Transposed) * (modMatrix) * (divisionVector) */
double computeDivisionsModularity(ModularityGroupMatrix *modMatrix, double * divisionVector );

/* divides Modularity Group Matrix to two Modularity Group Matrices according to the division vector
 * assigns pre-allocated pointer subModMatrices with the two sub-ModularityGroupMatrices*/
void divideModGroupMatrixToTwo (ModularityGroupMatrix *modMatrix, double *divisionVector, ModularityGroupMatrix **subModMatrices);

/* frees ModularityGroupMatrix */
void freeModularityGroupMatrix(ModularityGroupMatrix * modMatrix);


#endif
