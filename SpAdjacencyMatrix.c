#include "SpAdjacencyMatrix.h"
#include <stdlib.h>
#include "ErrorHandler.h"


/* adds row to pre-allocated SpAdjacencyMatrix
 * used only in the initial SpAdjacencyMatrix build */
LinkedList* addSpRow(int *indices, int rowNum, int rowSize);

/* creates sub-LinkedList of originalRow, corresponding to the rows and columns in verticesGroup */
LinkedList* createSubRow(LinkedList *originalRow, int *verticesGroup, int rowIndex, int groupSize);

/*------------------------------------------------------------------------------------------------------------*/
SpAdjacencyMatrix *allocateSpAdjMat(int n) {
    SpAdjacencyMatrix *matrix;
    LinkedList **rows;
    matrix = (SpAdjacencyMatrix *)malloc(sizeof(SpAdjacencyMatrix));
    verifyMemoryAllocation(matrix != NULL);
    rows = (LinkedList **)malloc(n * sizeof(LinkedList *));
    verifyMemoryAllocation(rows != NULL);
    matrix->rows = rows;
    matrix->n = n;
    return matrix;
}

SpAdjacencyMatrix * createAdjMatrixFromInput(char * filePath) {
    FILE * input;
    int i, tmp, dimension, rowSize, *indicesVector;
    LinkedList ** rows;
    SpAdjacencyMatrix * adjSpMatrix;
    input = fopen(filePath, "rb");
    verifyFileAccess(input != NULL);
    tmp = fread(&dimension, sizeof(int), 1, input);
    verifyFileRead(tmp == 1);
    verifyGraphContainsVertices(dimension != 0);
    adjSpMatrix = allocateSpAdjMat(dimension);
    rows = adjSpMatrix->rows;
    indicesVector = (int *)malloc(sizeof(int)*dimension);
    /* adding rows from the input file to the sparse matrix*/
    for (i = 0; i < dimension; i++) {
        tmp = fread(&rowSize, sizeof(int), 1, input);
        verifyFileRead(tmp == 1);
        verifyMemoryAllocation(indicesVector != NULL);
        tmp = fread(indicesVector, sizeof(int), rowSize, input);
        verifyFileRead( tmp == rowSize);
        *rows = addSpRow(indicesVector, i, rowSize);
        rows++;
    }
    free(indicesVector);
    fclose(input);
    return adjSpMatrix;
}

SpAdjacencyMatrix* createGroupSpAdjMatrix(SpAdjacencyMatrix *A, int *verticesGroup, int groupSize) {
    int i, *vertexIndex = verticesGroup;
    int verticesLeft = groupSize;
    LinkedList **originalRow = A->rows, *newSubRow;
    SpAdjacencyMatrix *subAdjMatrix = allocateSpAdjMat(groupSize);
    LinkedList **subRows = subAdjMatrix->rows;
    for (i = 0; i < A->n; i++) {
        if (verticesLeft > 0 && (*originalRow)->rowIndex == *vertexIndex) {
            newSubRow = createSubRow(*originalRow, verticesGroup, *vertexIndex, groupSize);
            *subRows = newSubRow;
            vertexIndex++;
            subRows++;
            verticesLeft--;
        }
        originalRow++;
    }
    return subAdjMatrix;
}

int getA(SpAdjacencyMatrix *A, int i, int j) {
    Node *currNode;
    LinkedList *row = A->rows[i];
    currNode = row->head;
    while(currNode != NULL && currNode->index < j) {
        currNode = currNode->next;
    }
    if (currNode != NULL && currNode->index == j) {
        return 1;
    }
    return 0;
}

void multSpAdjMatByVector(SpAdjacencyMatrix *A, double *vector, double *result) {
    int i;
    double sum;
    Node* currNode;
    LinkedList **rows = A->rows, **currRow = rows;
    for (i = 0; i < A->n; i++) {
        currNode = (*currRow)->head;
        sum = 0;
        while (currNode != NULL) {
            sum += vector[currNode->index];
            currNode = currNode->next;
        }
        currRow++;
        *result = sum;
        result++;
    }
}

void freeSpAdjMatrix(SpAdjacencyMatrix *A) {
    LinkedList **rows = A->rows, **currRow = rows;
    int rowNum;
    for (rowNum = 0; rowNum < A->n; rowNum++) {
        freeLinkedList(*currRow);
        currRow++;
    }
    free(rows);
    free(A);
}

/* -------------------------------private methods------------------------------- */
LinkedList* addSpRow(int *indices, int rowNum, int rowSize) {
    int i;
    LinkedList *row = createLinkedList();
    row->rowIndex = rowNum;
    for (i = 0; i < rowSize; i++) {
        addLast(row, *indices, *indices);
        indices++;
    }
    return row;
}

LinkedList* createSubRow(LinkedList *originalRow, int *verticesGroup, int rowIndex, int groupSize) {
    int verticesGroupIndex = 0 ;
    Node *originalRowNode = originalRow->head;
    LinkedList *newSubRow = createLinkedList();
    newSubRow->rowIndex = rowIndex;
    while (originalRowNode != NULL && verticesGroupIndex < groupSize) {
        if (originalRowNode->vertexNumber == *verticesGroup) {
            addLast(newSubRow, *verticesGroup, verticesGroupIndex);
            verticesGroup++;
            verticesGroupIndex++;
            originalRowNode = originalRowNode->next;
        }
        else if (originalRowNode->vertexNumber > *verticesGroup) {
            verticesGroup++;
            verticesGroupIndex++;
        }
        else {
            originalRowNode = originalRowNode->next;
        }
    }
    return newSubRow;
}
