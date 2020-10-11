#ifndef PROJECT_STACK_H
#define PROJECT_STACK_H

#include "ModularityGroupMatrix.h"


/*
 * Stack - Data structure of ModularityGroupMatrices elements
 */

/* ------------------------------------------------------------------------------------------------------------------*/

typedef struct Elem {
    ModularityGroupMatrix *value;
    struct Elem *next;
} Elem;

typedef struct Stack {
    int size;
    Elem *top;
} Stack;

/* initialize Stack */
Stack* initStack();

/* checks if the Stack is empty or not */
int isEmpty(Stack *pt);

/* adds an element to the top of the Stack */
void push(Stack *pt, ModularityGroupMatrix *value);

/* pops top element from the Stack */
ModularityGroupMatrix* pop(Stack *pt);

/* frees Stack */
void freeStack( Stack * stack);
#endif
