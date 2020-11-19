#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// "Preloaded Code" (do NOT modify!)
typedef struct node {
  int data;
  struct node *next;
} Node;

#define IS_DIGIT(x) (((x) >= '0') && ((x) <= '9'))

Node *parse(char *string) {

  int conv2num (int *tar,char *src) ;
  Node *newnode (Node *head, int data);
  void display (Node *list);

  int *buffer = malloc (1024 * sizeof(int));
  int end = conv2num (buffer,string);
  Node *tree = NULL;

  for (int i = 0; i < end; ++i) {
    tree = newnode (tree,buffer[i]);
  }

  buffer = NULL;
  free(buffer);
  return tree;
}

Node *newnode (Node *head, int data) {

  if (head == NULL) {
    head = malloc (sizeof(Node));
    head->data = data;
    head->next = NULL;
  } else
    head->next = newnode(head->next,data);

  return head;
}

long int conv2digit (char *src) {
  long int num = 0;
  char c;
  printf (" %s ",src);
  while ((c = *src++) != '\0') {
    if(IS_DIGIT(c))
      num = num * 10 + c - '0';
  }
  return num;
}

int conv2num (int *target,char *src) {
  long int conv2digit (char *src);
  int i = 0, j = 0, end = 0;
  char c, digit[10];

  while ((c = *src++) != '\0') {
    digit[i++] = c;

    if (c == '-') {
     digit[i] = '\0';
     target[j] = conv2digit (digit);
     i = 0, j++, end++;
    }

  }

return end;
}

void display (Node *list) {
  if(list) {
    printf ("%i -> ",list->data);
    display(list->next);
  }
}
