

#include "ErrorHandler.h"


void verifyMemoryAllocation(bool condition) {
    if (!condition) {
        printf("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
}

void verifyFileAccess(bool condition) {
    if (!condition) {
        printf("the file or path might not exist or could not be opened");
        exit(EXIT_FAILURE);
    }
}

void verifyFileRead(bool condition) {
    if (!condition) {
        printf("Could not read file.");
        exit(EXIT_FAILURE);
    }
}

void verifyFileWrite(bool condition) {
    if (!condition) {
        printf("Could not Write to file");
        exit(EXIT_FAILURE);
    }
}

void verifyDivisionByZero(bool condition) {
    if (!condition) {
        printf("Could not divide by zero");
        exit(EXIT_FAILURE);
    }
}

void verifyGraphContainsVertices(bool condition) {
    if (!condition) {
        printf("Graph is empty");
        exit(EXIT_FAILURE);
    }
}

void verifyGraphContainsEdges(bool condition) {
    if (!condition) {
        printf("Graph has no edges");
        exit(EXIT_FAILURE);
    }
}

void detectInfiniteLoop(bool condition) {
    if (!condition) {
        printf("Infinite loop detected");
        exit(EXIT_FAILURE);
    }
}


