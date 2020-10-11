

#include "ModularityGroupMatrix.h"
#include "ErrorHandler.h"
#include "SpAdjacencyMatrix.h"


/* initialize degrees vector for the vertices in g */
double* createInitialDegreesVector(SpAdjacencyMatrix *adjSpMatrix, int *group, int groupSize);

/* returns sub-degreesVector for the new group */
double* createGroupDegreesVector(int *originalGroup, double *originalDegreesVector, int *group, int groupSize);

/* updates the degreesSum (M) */
int computeDegreesSum(double *degreesVector, int dimension);

/* computes Fg */
double* computeF(ModularityGroupMatrix * modMatrix);

/* computes the modMatrix's Norm1 */
double computeNorm(ModularityGroupMatrix * modMatrix);

/*-----------------------------------------------------------------------------------------------------------*/

ModularityGroupMatrix* initFirstModularityMatrix(SpAdjacencyMatrix *adjMatrix, int *verticesGroup, int groupSize) {
    ModularityGroupMatrix *modMatrix;
    modMatrix = (ModularityGroupMatrix *)malloc(sizeof(ModularityGroupMatrix));
    verifyMemoryAllocation(modMatrix != NULL);
    modMatrix->adjMatrix = adjMatrix;
    modMatrix->verticesGroup = verticesGroup;
    modMatrix->groupSize = groupSize;
    modMatrix->degreesVector = createInitialDegreesVector(modMatrix->adjMatrix, verticesGroup, groupSize);
    modMatrix->M = computeDegreesSum( modMatrix->degreesVector, adjMatrix->n);
    modMatrix->f = computeF(modMatrix);
    verifyMemoryAllocation(modMatrix->f != NULL);
    modMatrix->norm = computeNorm(modMatrix);
    return modMatrix;
}

ModularityGroupMatrix* initSubModularityGroupMatrix(ModularityGroupMatrix *modMatrix, int *verticesGroup, int groupSize) {
    ModularityGroupMatrix *subModMatrix = (ModularityGroupMatrix *)malloc(sizeof(ModularityGroupMatrix));
    verifyMemoryAllocation(subModMatrix != NULL);
    subModMatrix->groupSize = groupSize;
    subModMatrix->verticesGroup = verticesGroup;
    subModMatrix->M = modMatrix->M;
    subModMatrix->adjMatrix = createGroupSpAdjMatrix(modMatrix->adjMatrix, verticesGroup, groupSize);
    subModMatrix->degreesVector = createGroupDegreesVector(modMatrix->verticesGroup, modMatrix->degreesVector, verticesGroup, groupSize);
    subModMatrix->f = computeF(subModMatrix);
    subModMatrix->norm = computeNorm(subModMatrix);
    return subModMatrix;
}

void multModMatrixByVector(ModularityGroupMatrix *modMatrix, double *vector, double *result, bool withF, bool shift) {
    double degreesVectorProduct, *vectorPtr, *fPtr, *adjMatProductPtr, *degreesVectorPtr, norm = modMatrix->norm;
    int i, groupSize = modMatrix->groupSize, M = modMatrix->M;
    double *adjMatProduct = (double *)malloc(sizeof(double)*groupSize);
    verifyMemoryAllocation(adjMatProduct != NULL);
    multSpAdjMatByVector(modMatrix->adjMatrix, vector, adjMatProduct); /* adjMatProduct = adjMatrix * vector */
    degreesVectorProduct = computeVectorsProduct(modMatrix->degreesVector, vector, groupSize) / M; /* degreesVectorProduct =  (degreesVector * vector) / M*/
    degreesVectorPtr = modMatrix->degreesVector;
    adjMatProductPtr = adjMatProduct;
    fPtr = modMatrix->f;
    vectorPtr = vector;
    for (i = 0; i < groupSize; i++) {
        *result =  (*adjMatProductPtr) - ((degreesVectorProduct) * (*degreesVectorPtr)) - (withF*(*fPtr) - shift*norm)*(*vectorPtr);
        result++;
        adjMatProductPtr++;
        degreesVectorPtr++;
        fPtr++;
        vectorPtr++;
    }
    free(adjMatProduct);
}

double computeDivisionsModularity(ModularityGroupMatrix *modMatrix, double *divisionVector ) {
    double result;
    double *productModMatDivisionVec = (double*) malloc(sizeof(double)*modMatrix->groupSize);
    verifyMemoryAllocation(productModMatDivisionVec != NULL);
    multModMatrixByVector(modMatrix, divisionVector, productModMatDivisionVec, 1, 0);
    result = computeVectorsProduct(divisionVector, productModMatDivisionVec, modMatrix->groupSize);
    free(productModMatDivisionVec);
    return 0.5*result;
}

void divideModGroupMatrixToTwo (ModularityGroupMatrix *modMatrix, double *divisionVector, ModularityGroupMatrix **subModMatrices) {
    int i, groupSize1, groupSize2, *verticesGroup1, *verticesGroup2, *verticesGroup1Ptr, *verticesGroup2Ptr;
    int *verticesGroup;
    double *divisionVectorPtr;
    ModularityGroupMatrix *subModMatrix1, *subModMatrix2;
    groupSize1 = 0;
    groupSize2 = 0;
    divisionVectorPtr = divisionVector;
    for (i = 0; i < modMatrix->groupSize; i++) {
        if (*divisionVectorPtr == 1) {
            groupSize1++;
        }
        else {
            groupSize2++;
        }
        divisionVectorPtr++;
    }
    if (groupSize1 == 0 || groupSize2 == 0) {
        subModMatrices[0] = modMatrix;
        subModMatrices[1] = NULL;
    }
    else {
        verticesGroup1 = (int *)malloc(sizeof(int) * groupSize1);
        verifyMemoryAllocation(verticesGroup1 != NULL);
        verticesGroup2 = (int *)malloc(sizeof(int) * groupSize2);
        verifyMemoryAllocation(verticesGroup2 != NULL);
        divisionVectorPtr = divisionVector;
        verticesGroup1Ptr = verticesGroup1;
        verticesGroup2Ptr = verticesGroup2;
        verticesGroup = modMatrix->verticesGroup;
        for (i = 0; i < modMatrix->groupSize; i++) {
            if (*divisionVectorPtr == 1) {
                *verticesGroup1Ptr = *verticesGroup;
                verticesGroup1Ptr++;
            }
            else {
                *verticesGroup2Ptr = *verticesGroup;
                verticesGroup2Ptr++;
            }
            divisionVectorPtr++;
            verticesGroup++;
        }
        subModMatrix1 = initSubModularityGroupMatrix(modMatrix, verticesGroup1, groupSize1);
        subModMatrix2 = initSubModularityGroupMatrix(modMatrix, verticesGroup2, groupSize2);
        subModMatrices[0] = subModMatrix1;
        subModMatrices[1] = subModMatrix2;
    }
}

void freeModularityGroupMatrix(ModularityGroupMatrix * modMatrix) {
    free(modMatrix->degreesVector);
    free(modMatrix->verticesGroup);
    free(modMatrix->f);
    freeSpAdjMatrix(modMatrix->adjMatrix);
    free(modMatrix);
}

/* -------------------------------------------private methods------------------------------------------- */

double* createInitialDegreesVector(SpAdjacencyMatrix *adjSpMatrix, int *group, int groupSize) {
    int i, originalDimension = adjSpMatrix->n, verticesLeft = groupSize;
    LinkedList **row;
    double *degreesVectorPtr;
    double *degreesVector = (double *) malloc(sizeof(double)*groupSize);
    verifyMemoryAllocation(degreesVector != NULL);
    degreesVectorPtr = degreesVector;
    row = adjSpMatrix->rows;
    for (i = 0; i < originalDimension; i++) {
        if(verticesLeft > 0 && (*row)->rowIndex == *group) {
            *degreesVectorPtr = (*row)->size;
            degreesVectorPtr++;
            group++;
            verticesLeft--;
        }
        row++;
    }
    return degreesVector;
}

double* createGroupDegreesVector(int *originalGroup, double *originalDegreesVector, int *group, int groupSize) {
    int verticesLeft = groupSize;
    double *degreesVectorPtr;
    double *degreesVector = (double *) malloc(sizeof(double)*groupSize);
    verifyMemoryAllocation(degreesVector != NULL);
    degreesVectorPtr = degreesVector;
    while (verticesLeft > 0) {
        if (*group == *originalGroup) {
            *degreesVectorPtr = *originalDegreesVector;
            group++;
            degreesVectorPtr++;
            verticesLeft--;
        }
        originalGroup++;
        originalDegreesVector++;
    }
    return degreesVector;
}

int computeDegreesSum( double * degreesVector, int dimension) {
    int i, degreesSum = 0;
    for (i = 0; i < dimension; i++) {
        degreesSum += (int)*degreesVector;
        degreesVector++;
    }
    verifyGraphContainsEdges(degreesSum != 0);
    return degreesSum;
}

double* computeF(ModularityGroupMatrix *modMatrix) {
    Node *currNode;
    LinkedList **rows, *currRow;
    int groupSize = modMatrix->groupSize, M = modMatrix->M;
    SpAdjacencyMatrix * adjSpMatrix = modMatrix->adjMatrix;
    int i, j, *vertex_i = modMatrix->verticesGroup, *vertex_j;
    double iDegree_divideByM, *vertex_j_degree, *fPtr, *degreesVectorPtr;
    double *f = (double *) malloc(sizeof(double) * groupSize);
    verifyMemoryAllocation(f != NULL);
    degreesVectorPtr = modMatrix->degreesVector;
    fPtr = f;
    rows = adjSpMatrix->rows;
    for (i = 0; i < groupSize; i++) {
        currRow = *rows;
        currNode = currRow->head;
        vertex_j = modMatrix->verticesGroup;
        *fPtr = 0;
        vertex_j_degree = modMatrix->degreesVector;
        iDegree_divideByM = (*degreesVectorPtr) / M;
        for (j = 0; j < groupSize; j++) {
            *fPtr -= iDegree_divideByM * (*vertex_j_degree);
            if (currNode != NULL && currNode->vertexNumber == *vertex_j) {
                *fPtr += 1;
                currNode = currNode->next;
            }
            vertex_j++;
            vertex_j_degree++;
        }
        degreesVectorPtr++;
        vertex_i++;
        fPtr++;
        rows++;
    }
    return f;
}

double computeNorm(ModularityGroupMatrix * modMatrix) {
    Node *currNode;
    LinkedList **rows, *currRow;
    double cellValue, sumRow, sumRowMax = -HUGE_VAL, *f = modMatrix->f, *degreesVector = modMatrix->degreesVector;
    double iDegree_divideByM, *vertex_j_degree, *degreesVectorPtr = modMatrix->degreesVector;
    int groupSize = modMatrix->groupSize, *group = modMatrix->verticesGroup;
    int i, j, *vertex_i = group, *vertex_j, M = modMatrix->M;
    rows = modMatrix->adjMatrix->rows;
    for (i = 0; i < groupSize; i++) {
        currRow = *rows;
        currNode = currRow->head;
        vertex_j = group;
        vertex_j_degree  = degreesVector;
        sumRow = 0;
        iDegree_divideByM = *degreesVectorPtr / M;
        for ( j = 0; j < groupSize; j++) {
            cellValue =  -(iDegree_divideByM * (*vertex_j_degree));
            if (currNode != NULL && currNode->vertexNumber == *vertex_j) {
                cellValue += 1;
                currNode = currNode->next;
            }
            if ( i == j ) {
                cellValue -= *f;
                f++;
            }
            sumRow += fabs(cellValue);
            vertex_j++;
            vertex_j_degree++;
        }
        if (sumRowMax < sumRow) {
            sumRowMax = sumRow;
        }
        vertex_i++;
        degreesVectorPtr++;
        rows++;
    }
    return sumRowMax;
}
