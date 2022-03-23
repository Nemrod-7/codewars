#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// In the preloaded section are some functions that can help.
// They can be used as a small library.
// There is no file to include, only the templates below.

struct node {
    int data;
    struct node *next;
};
struct list {
    size_t sz;
    struct node *head;
};

struct list* createList();

// push data at the head of the list
void insertFirst(struct list* l, int data);

struct list* reverse(struct list* l);

void listFree(struct list* l);

unsigned count_k (unsigned num) {

    int i = 1, count = 0, div;

    i = 2;
    while (num > 1) {
        while (num % i == 0) {
            num /= i;
            count++;
        }
        i++;
    }

    return count;
}
int consecKprimes(int k, struct list* l) {

    struct node *now= l->head;
    int count = 0;
    bool flag = false;

    while (now) {
        if (count_k (now->data) == k) {
            if (flag == true)
                count++;
            flag = true;
        } else
            flag = false;

        now = now->next;
    }

    return count;
}
