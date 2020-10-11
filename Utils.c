
#include "Utils.h"


double computeDotProduct(double * vector, int dimension) {
    double dotProduct = 0;
    int i;
    for (i = 0; i < dimension; i++) {
        dotProduct += (*vector) * (*vector);
        vector++;
    }
    return sqrt(dotProduct);
}

void normalizeVector(double *vector, double dotProduct, int dimension) {
    int i;
    verifyDivisionByZero(dotProduct != 0);
    for (i = 0; i < dimension; i++) {
        *vector = (*vector) / (dotProduct);
        vector++;
    }
}

void initializeRandomVector(double * vector, int dimension) {
    int i;
    for (i = 0; i < dimension; i++) {
        *vector = rand();
        vector++;
    }
}

double computeVectorsProduct(double *vector1, double *vector2, int dimension)   {
    int i;
    double result = 0;
    for ( i = 0; i < dimension; i++) {
        result += (*vector1) * (*vector2);
        vector1++;
        vector2++;
    }
    return result;
}

bool areEqual(double *vector, double *nextVector, int dimension) {
    int i;
    double diff;
    for (i = 0; i < dimension; i++) {
        diff = fabs(*nextVector - *vector);
        if (IS_POSITIVE(diff)) {
            return 0;
        }
        vector++;
        nextVector++;
    }
    return 1;
}

void initDoublesVectorWithDefaultValue(double *vector, int size, double defaultValue) {
    int i;
    for (i = 0; i < size; i++) {
        *vector = defaultValue;
        vector++;
    }
}

void initBoolsVectorWithZeros(bool *vector, int size) {
    int i = 0;
    while (i < size) {
        *vector = 0;
        vector++;
        i++;
    }
}

void initGroupVector(int *vector, int n) {
    int i;
    for (i = 0; i < n; i++) {
        *vector = i;
        vector++;
    }
}

