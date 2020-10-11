#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Stack.h"
#include "ErrorHandler.h"



/*constructor for Stack element*/
Elem *createStackElem(ModularityGroupMatrix *value);

/* ---------------------------------------------------------------------------------------------------- */

Stack* initStack() {
    Stack *pt = (Stack*)malloc(sizeof(Stack));
    verifyMemoryAllocation(pt != NULL);
    pt->size = 0;
    pt->top = NULL;
    return pt;
}

int isEmpty(Stack *pt) {
    return pt->size == 0;
}

void push(Stack *pt, ModularityGroupMatrix *value) {
    Elem *newElem = createStackElem(value);
    newElem->next = pt->top;
    pt->top = newElem;
    pt->size++;
}

ModularityGroupMatrix * pop(Stack *pt) {
    ModularityGroupMatrix *value = NULL;
    Elem *poped;
    if (isEmpty(pt)) {
        printf("You can not pop an empty Stack!");
    }
    else {
        poped = pt->top;
        value = pt->top->value;
        pt->top = pt->top->next;
        pt->size--;
        free(poped);
    }
    return value;
}

void freeStack( Stack * stack) {
    while(!isEmpty(stack)) {
        pop(stack);
    }
    free(stack);
}

/* -------------------------------------------private methods------------------------------------------- */

Elem *createStackElem(ModularityGroupMatrix *value) {
    Elem *newElem = (Elem *)malloc(sizeof(Elem));
    verifyMemoryAllocation(newElem != NULL);
    assert(newElem!=NULL);
    newElem->value = value;
    newElem->next = NULL;
    return newElem;
}
