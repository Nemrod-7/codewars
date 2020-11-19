#include <stdbool.h>
#include <stdlib.h>

typedef struct node {
    int data;
    struct node *next;
} Node;

typedef struct {
    Node *front, *back;
} Queue;

int main () {


}

int front (const Queue *Q) { return Q->front->data;}
bool empty (const Queue *Q) { return (!Q->front || !Q->back);}
void push (Queue *Q, int data) {
    //bool queue_is_empty(const Queue *Q);
    Node *cell = malloc (sizeof (Node));
    cell->data = data;
    cell->next = NULL;

    if (empty(Q))
        Q->back = Q->front = cell;
    else {
        Q->back = Q->back->next = cell;
    }
}
int pop (Queue *Q) {

    Node *frontnode = Q->front;
    int value = front (Q);

    Q->front = Q->front->next;

    free (frontnode);
    return value;
}
