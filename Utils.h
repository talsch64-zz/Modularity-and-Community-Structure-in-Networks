

#ifndef PROJECT_UTILS_H
#define PROJECT_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include "SpAdjacencyMatrix.h"
#include <math.h>
#include "ErrorHandler.h"
#include "stdbool.h"
#include "ModularityGroupMatrix.h"
#define IS_POSITIVE(X) ((X) > 0.00001)
#define ITARATION_BOUND(X) (X*X + 10000*X + 500000)

/*
 * Utils: Module for useful utilities and linear algebraic operations
 */

/*------------------------------------------------------------------------------------------------------------------*/

/* returns the dot product of the given vector */
double computeDotProduct(double * vector, int dimension);

/* modify vector by normalizing */
void normalizeVector(double * vector, double dotProduct, int dimension);

/* assign pre-allocated vector with random values*/
void initializeRandomVector(double * vector, int dimension);

/* returns the product of vector1 and vector2 */
double computeVectorsProduct(double *vector1, double *vector2, int dimension);

/* return 0 if exist i such that diff(vector[i] , nextVector[i]) is positive, else return 1 */
bool areEqual(double *vector, double *nextVector,int dimension);

/* initializes pre-allocated doubles vector with the given default value */
void initDoublesVectorWithDefaultValue(double * vector, int size, double defaultValue);

/* initializes pre-allocated bool vector with zeros */
void initBoolsVectorWithZeros(bool *vector, int size);

/* initializes pre-allocated array with the values 0,1,2...,n */
void initGroupVector (int *vector, int n);



#endif
