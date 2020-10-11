
#ifndef PROJECT_LINKEDLIST_H
#define PROJECT_LINKEDLIST_H

/* LinkedList Data Structure
 * Represents a row and each Node represent a vertex in the graph
 *
 * Struct Node members:
     * vertexNumber: the original vertex number
     * index: the vertex index relative to the subGroup
     * next: the next Node neighbor
 *
 * Struct LinkedList members:
    * head
    * tail
    * rowIndex: the index of the row
    * size
 */

/* ------------------------------------------------------------------------------------------------------------------*/

typedef struct Node {
    int vertexNumber;
    int index;
    struct Node *next;

} Node;

typedef struct LinkedList {
    Node *head;
    Node *tail;
    int rowIndex;
    int size;
} LinkedList;

/*constructor for Node*/
Node *createNode(int value, int index);

/* constructor for LinkedLis */
LinkedList *createLinkedList();

/* add new Node at the end of the list */
void addLast (LinkedList * list, int value, int index);

/* free LinkedList list*/
void freeLinkedList(LinkedList* list);

#endif
