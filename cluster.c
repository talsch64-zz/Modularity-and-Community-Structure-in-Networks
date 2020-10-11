#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "SpAdjacencyMatrix.h"
#include <math.h>
#include "Stack.h"
#include "ErrorHandler.h"
#include "ModularityGroupMatrix.h"
#include "Utils.h"

SpAdjacencyMatrix * createAdjMatrixFromInput(char * filePath);
double * findEigenVector(ModularityGroupMatrix * modMatrix);
double computeEigenValue (ModularityGroupMatrix *modMatrix, double* eigenVector);
double *computeDivisionVector (double *eigenVector, int dimension);
void optimizeDivision (ModularityGroupMatrix *modMatrix, double *divisionVector);
void writeClustersToOutputFile(char *filePath, Stack *clusters);

/*--------------------------------------------------------------------------------------------------------------------*/

int main(int argc, char *argv[]) {
    clock_t start, end;
    register SpAdjacencyMatrix *adjSpMatrix;
    register ModularityGroupMatrix * modMatrix, *subModMatrix1, *subModMatrix2, **subModMatrices;
    register int groupSize, *verticesGroup;
    register double *eigenVector, eigenValue ,*divisionVector, modularity;
    struct Stack *P, *O;
    argc = argc + 0;
    srand(time(NULL));
    start = clock();
    subModMatrices = (ModularityGroupMatrix **)malloc(sizeof(ModularityGroupMatrix*)*2);
    verifyMemoryAllocation(subModMatrices != NULL);
    adjSpMatrix = createAdjMatrixFromInput(argv[1]);
    P = initStack();
    O = initStack();
    groupSize = adjSpMatrix->n;
    verticesGroup = (int *)malloc(sizeof(int)*groupSize);
    verifyMemoryAllocation(verticesGroup != NULL);
    initGroupVector(verticesGroup, groupSize);
    modMatrix = initFirstModularityMatrix(adjSpMatrix, verticesGroup, groupSize);
    push(P, modMatrix);
    while (!isEmpty(P)) {
        modMatrix = pop(P);
        groupSize = modMatrix->groupSize;
        eigenVector = findEigenVector(modMatrix);
        eigenValue = computeEigenValue(modMatrix, eigenVector);
        if (!IS_POSITIVE(eigenValue)) {
            /* The group g is indivisible */
            push(O, modMatrix);
            free(eigenVector);
            continue;
        }
        divisionVector = computeDivisionVector(eigenVector, groupSize);
        modularity = computeDivisionsModularity(modMatrix, divisionVector);
        if (!IS_POSITIVE(modularity)) {
            /* The group g is indivisible */
            initDoublesVectorWithDefaultValue(divisionVector, groupSize, 1);
        }
        optimizeDivision(modMatrix, divisionVector);
        divideModGroupMatrixToTwo(modMatrix, divisionVector, subModMatrices);
        subModMatrix1 = subModMatrices[0];
        subModMatrix2 = subModMatrices[1];
        if (subModMatrix2 == NULL) {
            push(O, modMatrix); /* The optimal division is trivial */
        }
        else {
            subModMatrix1->groupSize == 1 ? push(O, subModMatrix1) : push(P, subModMatrix1);
            subModMatrix2->groupSize == 1 ? push(O, subModMatrix2) : push(P, subModMatrix2);
            freeModularityGroupMatrix(modMatrix);
        }
        free(divisionVector);
        free(eigenVector);
    }
    writeClustersToOutputFile(argv[2], O);
    free(subModMatrices);
    freeStack(O);
    freeStack(P);
    end = clock();
    printf("\nExecution took %f seconds\n",
           ((double) (end - start) / CLOCKS_PER_SEC));
    return 0;
}

/*--------------------------------------------------------------------------------------------------------------------*/

/* Create output with the clusters found according to the instructed format*/
void writeClustersToOutputFile(char *filePath, Stack *clusters) {
    FILE* output;
    ModularityGroupMatrix *modMatrix;
    int tmp, groupSize;
    output = fopen(filePath, "wb");
    verifyFileAccess(output != NULL);
    tmp = fwrite(&(clusters->size), sizeof(int), 1, output);
    verifyFileWrite(tmp == 1);
    while (!isEmpty(clusters)) {
        modMatrix = pop(clusters);
        groupSize = modMatrix->groupSize;
        tmp = fwrite(&(groupSize), sizeof(int), 1, output);
        verifyFileWrite(tmp == 1);
        tmp = fwrite(modMatrix->verticesGroup, sizeof(int), groupSize, output);
        verifyFileWrite(tmp == groupSize);
        freeModularityGroupMatrix(modMatrix);
    }
    fclose(output);
}

/* Compute the eigen vector of modMatrix corresponding to the largest eigenvalue */
double *findEigenVector(ModularityGroupMatrix * modMatrix) {
    register double *vector, *nextVector, *tmpVector, dotProduct;
    int dimension = modMatrix->groupSize, iterationNumber = 0, iterationBound = ITARATION_BOUND(dimension);
    vector = (double*)malloc(sizeof(double)*dimension);
    verifyMemoryAllocation(vector!=NULL);
    nextVector = (double*)malloc(sizeof(double)*dimension);
    verifyMemoryAllocation(nextVector!=NULL);
    initializeRandomVector(vector, dimension);
    do {
        multModMatrixByVector(modMatrix, vector, nextVector, 1, 1); /* nextVector = (shifted modMatrix) * (vector) */
        dotProduct = computeDotProduct(nextVector, modMatrix->groupSize);
        normalizeVector(nextVector, dotProduct, modMatrix->groupSize); /* nextVector = (nextVector) / ( ||nextVector|| ) */
        tmpVector = vector;
        vector = nextVector;
        nextVector = tmpVector;
        iterationNumber++;
        detectInfiniteLoop(iterationNumber < iterationBound);
    }
    while (!areEqual(vector, nextVector, dimension));
    free(nextVector);
    return vector;
}

/* Computes the eigenValue corresponding to the eigen vector
 * eigenValue = (bk * (B^[g]_shifted * bk) / bk*bk) - ||B^[g]|| */
double computeEigenValue (ModularityGroupMatrix *modMatrix, double* eigenVector) {
    int dimension = modMatrix->groupSize;
    double nominator, eigenValue;
    double *productModMatEigenVec = (double*)malloc(sizeof(double)*dimension);
    verifyMemoryAllocation(productModMatEigenVec != NULL);
    multModMatrixByVector(modMatrix, eigenVector, productModMatEigenVec, 1, 1);
    nominator = computeVectorsProduct(eigenVector, productModMatEigenVec, dimension);
/*  denominator is always 1 */
    eigenValue = nominator - modMatrix->norm;
    free(productModMatEigenVec);
    return eigenValue;
}

/* Computes the division vector S according to the eigen vectors values */
double *computeDivisionVector (double *eigenVector, int dimension) {
    int i;
    double *divisionVectorPtr;
    double *divisionVector = (double*)malloc((sizeof(double)) * dimension);
    verifyMemoryAllocation(divisionVector != NULL);
    divisionVectorPtr = divisionVector;
    for ( i = 0; i < dimension; i++) {
        if (IS_POSITIVE(*eigenVector)) {
            *divisionVectorPtr = 1;
        }
        else {
            *divisionVectorPtr = -1;
        }
        divisionVectorPtr++;
        eigenVector++;
    }
    return divisionVector;
}

/* Optimize the division according to algorithm 4 (with optimized calculations for the scores values) */
void optimizeDivision (ModularityGroupMatrix *modMatrix, double *divisionVector) {
    bool *unmoved, *unmovedPtr;
    int maxScoreIndex = 0, k, groupSize = modMatrix->groupSize, i, lastMovedIndex = 0, M = modMatrix->M, *movedVertices, *movedVerticesPtr;
    double maxScore, deltaModularity, *scores, *degreesVector = modMatrix->degreesVector;
    double improve = 0, maxImprovement, maxImprovementIteration = 0, *B_s_product, lastMovedDegree_dividedByM;
    double *B_s_productPtr, *degreesVectorPtr, *scoresPtr, *divisionVectorPtr, lastMovedIndexSign;
    movedVertices = (int *)malloc(sizeof(int) * groupSize);
    verifyMemoryAllocation(movedVertices != NULL);
    unmoved = (bool *)malloc(groupSize * sizeof(bool));
    verifyMemoryAllocation(unmoved != NULL);
    scores = (double *)malloc(sizeof(double)*groupSize);
    verifyMemoryAllocation(scores != NULL);
    B_s_product = (double *)malloc(sizeof(double)*groupSize);
    verifyMemoryAllocation(B_s_product != NULL);
/*  ----- repeat until there is no improvement to the modularity ----- */
    do {
        initBoolsVectorWithZeros(unmoved, groupSize);
        movedVerticesPtr = movedVertices;
        for (i = 0; i < groupSize; i++) {
            maxScore = -HUGE_VAL;
            divisionVectorPtr = divisionVector;
            degreesVectorPtr = degreesVector;
            scoresPtr = scores;
            unmovedPtr = unmoved;
            if (i == 0) {
                multModMatrixByVector(modMatrix, divisionVector, B_s_product, 0, 0);
                B_s_productPtr = B_s_product;
                /* if no vertex was moved yet compute the scores according to:
                * scores[k] = -2 * (s[k] * B_s_product[k] + degreesVector[k]*degreesVector[k]/M) */
                for (k = 0; k < groupSize; k++) {
                    *scoresPtr = -2*((*divisionVectorPtr)*(*B_s_productPtr) + (*degreesVectorPtr) * (*degreesVectorPtr) / M);
                    if (*scoresPtr > maxScore) {
                        maxScore = *scoresPtr;
                        maxScoreIndex = k;
                    }
                    scoresPtr++; divisionVectorPtr++; degreesVectorPtr++; B_s_productPtr++;
                }
            }
            else {
                /* if a vertex has been already moved, update scores according to:
                 * if k is the index of the last vertex that was moved: scores[k] = -scores[k]
                 * else scores[k] -= 4*(s[k]*s[lastMovedIndex]*B[k, lastMovedIndex]); */
                lastMovedDegree_dividedByM = degreesVector[lastMovedIndex] / M;
                lastMovedIndexSign = divisionVector[lastMovedIndex];
                for (k = 0; k < groupSize; k++) {
                /*  if k == lastMovedIndex it is never used
                 * if k is a moved vertex then it is never used either*/
                    if (*unmovedPtr == 0) {
                        /*scores[k] -= 4*(s[k]*s[lastMovedIndex]*B[k, lastMovedIndex]); */
                        *scoresPtr -= 4*((*divisionVectorPtr)*lastMovedIndexSign*(getA(modMatrix->adjMatrix, k, lastMovedIndex)
                                                                              - (*degreesVectorPtr)*lastMovedDegree_dividedByM));
                        if (*scoresPtr > maxScore) {
                            maxScore = *scoresPtr;
                            maxScoreIndex = k;
                        }
                    }
                    scoresPtr++; divisionVectorPtr++; degreesVectorPtr++; unmovedPtr++;
                }
            }
            /* ----- move the vertex with the maximal modularity to the other group and update maxImprovement ----- */
            lastMovedIndex = maxScoreIndex;
            unmoved[maxScoreIndex] = 1;
            divisionVector[maxScoreIndex] *= -1;
            *movedVerticesPtr = maxScoreIndex;
            movedVerticesPtr++;
            improve = (i == 0 ? maxScore : improve + maxScore);
            if (i == 0 || improve > maxImprovement) {
                maxImprovement = improve;
                maxImprovementIteration = i;
            }
        }
        /* ----- update divisionVector accordingly to the maximum improvement ----- */
        movedVerticesPtr--;  /* movedVerticesPtr = movedVertices + groupSize - 1; */
        for ( i = groupSize - 1; i > maxImprovementIteration; i--) {
            divisionVector[*movedVerticesPtr] *= -1;
            movedVerticesPtr--;
        }
        /* ----- assign new modularity ----- */
        deltaModularity = (maxImprovementIteration == groupSize - 1 ? 0 : maxImprovement);
    }
    while (IS_POSITIVE(deltaModularity));
    free(unmoved); free(B_s_product); free(scores); free(movedVertices);
}

