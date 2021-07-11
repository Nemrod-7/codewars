#include <stdio.h>
#include <stddef.h>

struct node {
    int data;
    struct node *next;
};

size_t len_r (const struct node *list) {
    if (list == NULL) return 0;
    return 1 + len_r (list->next);
}
int main () {

}

/*
int lenR (int *array) {

  if (array == '\0')
  return 0;
  else if (*array != '\0')
  return (1 + lenR (++array));
}
*/
