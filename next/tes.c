#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int value, visit;
    int nedge;
    struct node **edge;
} vertex;

int n, n_sqr, *sqr, *path;
vertex *graph;

int comp (const void *a, const void *b) {
  vertex *num_a = *(vertex * const *)a, *num_b = *(vertex * const *)b;
  if (num_a->nedge != num_b->nedge) {
      return num_a->nedge - num_b->nedge;
  }
  return num_a->value - num_b->value;
}
void eval_number (vertex *node) {

    node->nedge = 0;
    for (int i = 0; i < n_sqr; i++) {
        int value = sqr[i] - node->value;

        if (value >= 1 && value <= n && value != node->value && !graph[value-1].visit) {
            node->edge[node->nedge++] = graph + value - 1;
        }
    }
}
void init (vertex *curr, int value, vertex **edge) {
    curr->value = value;
    curr->visit = 0;
    curr->edge = edge;
}

int backtrack (int index, vertex *curr) {

    if (index == n) return 1;
    if (curr->nedge == 0) return 0;

    int found = 0, i;

    for (i = 0; i < curr->nedge; i++) {
        eval_number (curr->edge[i]);
    }

    qsort (curr->edge, (size_t) curr->nedge, sizeof (vertex *), comp);

    for (i = 0; i < curr->nedge && !found; i++) {
        curr->edge[i]->visit = 1;
        path[index] = curr->edge[i]->value;
        found = backtrack (index + 1, curr->edge[i]);
        curr->edge[i]->visit = 0;
    }

    return found;
}
void shownode (vertex *curr) {

    printf ("%d -> ", curr->value);
    for (int i = 0; i < curr->nedge; i++) {
        printf ("%d ", curr->edge[i]->value);
    }

    printf ("\n");
}
int main (void) {

    int first_max, found = 0, i;
    vertex **buffer, **first_edge;

    n = 50;

    for (i = 2; i * i < n * 2; i++);

    n_sqr = i - 2;

    sqr = malloc (sizeof (int) * (size_t) n_sqr);
    graph = malloc (sizeof (vertex) * (size_t) n);
    buffer = malloc (sizeof(vertex *) * (size_t)(n * n_sqr));
    path = malloc(sizeof(int)*(size_t)n);

    for (i = 2; i * i < n * 2; i++) {
        sqr[i - 2] = i * i;
    }

    for (i = 0; i < n; i++) {
        init (graph + i, i + 1, buffer + i * n_sqr);
    }

    first_max = n / 2;
    if (n % 2 == 1) {
        first_max++;
    }

    first_edge = malloc (sizeof(vertex *)*(size_t)first_max);

    for (i = 0; i < first_max; i++) {
        eval_number (graph + i);
        first_edge[i] = graph + i;
        //shownode (graph + i);
    }

    qsort (first_edge, (size_t)first_max, sizeof(vertex *), comp);


    for (i = 0; i < first_max && !found; i++) {
        first_edge[i]->visit = 1;
        path[0] = first_edge[i]->value;
        //printf ("%i ", first_edge[i]->value);
        found = backtrack (1, first_edge[i]);
        first_edge[i]->visit = 0;
    }
    printf ("\n");
    /*
    */
    if (found) {
        for (i = 0; i < n; i++) {
            printf("%d ", path[i]);
        }
    } else {
        puts("Not possible");
    }
    free(path);
    free(first_edge);
    free(buffer);
    free(graph);
    free(sqr);

    if (found) {
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}
