

#include "LinkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include "ErrorHandler.h"

/* free all nodes from listHead's LinkedList */
void freeListNodes(Node *listHead);

/*------------------------------------------------------------------------------------------------------*/

Node *createNode(int value, int index) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    verifyMemoryAllocation(newNode != NULL);
    newNode->vertexNumber = value;
    newNode->index = index;
    newNode->next = NULL;
    return newNode;
}

LinkedList *createLinkedList() {
    LinkedList *row = (LinkedList *)malloc(sizeof(LinkedList));
    verifyMemoryAllocation(row != NULL);
    row->head = NULL;
    row->tail = NULL;
    row->size = 0;
    row->rowIndex = -1;
    return row;
}

void addLast (LinkedList * list, int value, int index) {
    Node * newNode = createNode(value, index);
    if (list->size == 0) {
        list->head = newNode;
        list->tail = newNode;

    }
    else {
        list->tail->next = newNode;
        list->tail = newNode;
    }
    list->size++;
}

void freeListNodes(Node *listHead) {
    if (listHead != NULL) {
        freeListNodes(listHead->next);
        free(listHead);
    }
}

void freeLinkedList(LinkedList* list) {
    freeListNodes(list->head);
    free(list);
}
