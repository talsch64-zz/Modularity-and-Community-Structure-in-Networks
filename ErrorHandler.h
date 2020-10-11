
#ifndef PROJECT_ERRORHANDLER_H
#define PROJECT_ERRORHANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * ErrorHandler - verifies operations, exits with failure if operation failed
 */

/*------------------------------------------------------------------------------------------------------------------*/
/* verify memory allocation success */
void verifyMemoryAllocation(bool condition);

/* verify file access success */
void verifyFileAccess(bool condition);

/* verify file read success */
void verifyFileRead(bool condition);

/* verify file write success */
void verifyFileWrite(bool condition);

/*verify the denominator is not zero*/
void verifyDivisionByZero(bool condition);

/* verify graph contains vertices */
void verifyGraphContainsVertices(bool condition);

/* verify graph contains edges */
void verifyGraphContainsEdges(bool condition);

/* check if entered an infinite loop */
void detectInfiniteLoop(bool condition);


#endif
