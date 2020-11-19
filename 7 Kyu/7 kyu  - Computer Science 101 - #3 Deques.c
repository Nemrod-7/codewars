#include <stdlib.h>
#include <stdbool.h>

// Key definitions
// Please do not modify them - attempts to do so
// may lead to unexpected behavior

typedef struct node {
    int data;
    struct node *prev, *next;
} Node;

typedef struct {
    Node *front, *back;
} Deque;
bool deque_is_empty(const Deque *deque) {return (!deque->front || !deque->back);
 }

void deque_push_front(Deque *deque, int data) {

    Node *cell = malloc (sizeof (Node));
    cell->data = data;
    cell->prev = cell->next = NULL;

    if (deque_is_empty(deque))
        deque->front = deque->back = cell;
    else
        cell->next = deque->front;
        deque->front = deque->front->prev = cell;
    //deque->front = cell;
 }
int deque_pop_front(Deque *deque) {

    int value = deque->front->data;
    Node *p = deque->front;
    deque->front = p->next;

    if (deque->front)
        deque->front->prev = NULL;

    free(p);

    return value;
}
int deque_peek_front(const Deque *deque) { return deque->front->data; }

void deque_push_back(Deque *deque, int data) {

    Node *cell = malloc (sizeof (Node));
    cell->data = data;
    cell->prev = cell->next = NULL;

    if (deque_is_empty (deque)) {
        deque->front = deque->back = cell;
    }
    else
        cell->prev = deque->back;
        deque->back = deque->back->next = cell;
 }
int deque_pop_back(Deque *deque) {

    int value = deque->back->data;
    Node *p = deque->back;
    deque->back = p->prev;

    if (deque->back)
        deque->back->next = NULL;
    else
        deque->back = deque->front = NULL;

    free (p);

    return value;
 }
int deque_peek_back(const Deque *deque) {return deque->back->data; }
