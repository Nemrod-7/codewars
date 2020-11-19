#include <stdbool.h>
#include <stdlib.h>



typedef struct node {
    int data;
    struct node *next;
} Node;


typedef struct {
    Node *root;
} Stack;

void stack_push(Stack *stack, int data);
int stack_pop(Stack *stack);
int stack_peek(const Stack *stack);
bool stack_is_empty(const Stack *stack);

void stack_push(Stack *stack, int data) {

    Node *cell = stack->root;
    stack->root = malloc (sizeof (Node));
    stack->root->data = data;
    stack->root->next = cell;
}

int stack_pop(Stack *stack) {

    Node *cell = stack->root;
    int value = stack->root->data;
    stack->root = stack->root->next;

    free (cell);

    return value;
}

int stack_peek(const Stack *stack) {
    return (stack_is_empty(stack)) ? 0 : stack->root->data;
}

bool stack_is_empty(const Stack *stack) {
    return ((stack->root == NULL) ? true : false);
}
