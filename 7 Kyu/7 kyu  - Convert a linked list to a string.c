#include <stdio.h>
#include <string.h>


typedef struct node {
  int data;
  struct node *next;
} Node;

char buffer[1024];

char *stringify(Node *list) {

  char *tmp = buffer;
  *buffer = '\0';

  while (list) {
   tmp += sprintf (tmp,"%i -> ",list->data);
    list = list->next;
 }

     strcat (buffer,"NULL");

  return buffer;
}
