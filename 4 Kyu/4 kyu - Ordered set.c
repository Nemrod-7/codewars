#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int (*Comparator)(const void *, const void *);
typedef char *(*Stringizer)(const void *);

enum {BLACK, RED};

typedef struct _node {
    const void *data;
    bool color;
    struct _node *left, *right, *head;
} Node;

typedef struct set {
    Node *root;
    Comparator mode;
    Stringizer deb;
    size_t size;
} Set;

Node empty = { 0, BLACK, NULL , NULL , NULL}, *NIL = &empty;
/////////////////////////node manipulation////////////////////////////
Node *newnode (const void *data) {
    Node *new = malloc (sizeof (Node));
    new->data = data;
    new->left = new->right = new->head = NIL;
    new->color = RED;
    return new;
}
Node *parent (Node *curr) { return curr != NULL ? curr->head : NULL;}
Node *gparent (Node *curr) { return parent (parent(curr));}

void rotateleft (Set *S, Node *x) {

    Node *y = x->right;

    x->right = y->left;

    if (y->left != NIL)
        y->left->head = x;

    if (y != NIL)
        y->head = parent(x);

    if (parent(x)) {
        if (x == parent(x)->left)
            parent(x)->left = y;
        else
            parent(x)->right = y;
    } else {
        S->root = y;
    }

    y->left = x;
    if (x != NIL)
        x->head = y;
}
void rotateright (Set *S, Node *x) {
    Node *y = x->left;

    x->left = y->right;

    if (y->right != NIL)
        y->right->head = x;

    if (y != NIL)
        y->head = parent(x);


    if (parent(x)) {
        if (x == parent(x)->right)
            parent(x)->right = y;
        else
            parent(x)->left = y;
    } else {
        S->root = y;
    }

    y->right = x;
    if (x != NIL)
        x->head = y;
}

void mk_balance (Set *S, Node *curr) {

    while (curr != S->root && parent(curr)->color == RED) {

        if (parent(curr) == gparent(curr)->left) {

            Node *y = gparent(curr)->right;

            if (y->color == RED) {
                parent(curr)->color = BLACK;
                y->color = BLACK;
                gparent(curr)->color = RED;
                curr = gparent(curr);

            } else {

                if (curr == parent(curr)->right) {
                    curr = parent(curr);
                    rotateleft (S, curr);
                }

                parent(curr)->color = BLACK;
                gparent(curr)->color = RED;
                rotateright (S, gparent(curr));
            }

        } else {

            Node *y = gparent(curr)->left;

            if (y->color == RED) {
                parent(curr)->color = BLACK;
                y->color = BLACK;
                gparent(curr)->color = RED;
                curr = gparent(curr);
            } else {

                if (curr == parent(curr)->left) {
                    curr = parent(curr);
                    rotateright (S, curr);
                }

                parent(curr)->color = BLACK;
                gparent(curr)->color = RED;
                rotateleft (S, gparent(curr));
            }
        }
    }

    S->root->color = BLACK;
}
void rebalance (Set *S, Node *curr) {

  while (curr != S->root && curr->color == BLACK) {

    if (curr == curr->head->left) {

      Node *w = curr->head->right;

      if (w->color == RED) {
          w->color = BLACK;
          curr->head->color = RED;
          rotateleft (S, curr->head);
          w = curr->head->right;
      }

      if (w->left->color == BLACK && w->right->color == BLACK) {
          w->color = RED;
          curr = curr->head;
      } else {

          if (w->right->color == BLACK) {
              w->left->color = BLACK;
              w->color = RED;
              rotateright (S, w);
              w = curr->head->right;
          }

          w->color = curr->head->color;
          curr->head->color = BLACK;
          w->right->color = BLACK;
          rotateleft (S, curr->head);
          curr = S->root;
      }
    } else {

      Node *w = curr->head->left;

      if (w->color == RED) {
          w->color = BLACK;
          curr->head->color = RED;
          rotateright  (S, curr->head);
          w = curr->head->left;
      }

      if (w->right->color == BLACK && w->left->color == BLACK) {
          w->color = RED;
          curr = curr->head;
      } else {

          if (w->left->color == BLACK) {
              w->right->color = BLACK;
              w->color = RED;
              rotateleft (S, w);
              w = curr->head->left;
          }

          w->color = curr->head->color;
          curr->head->color = BLACK;
          w->left->color = BLACK;
          rotateright  (S, curr->head);
          curr = S->root;
      }
    }
  }
  curr->color = BLACK;

}
/////////////////////////////B-tree wraper////////////////////////////
Set *set_initialize (Comparator cmp, Stringizer str) { // initialize a set
    //printf ("\nseq_start\n");
    Set *S = malloc (sizeof(Set));
    S->mode = cmp;
    S->deb = str;
    S->root = NIL;
    S->size = 0;
    return S;
}
void set_destroy (Set *S) { // destroy the set
    Node *curr = S->root, *left, *right;
    //printf ("\nseq_end\n");
    while (curr != NIL) {
        left = curr->left;
        if (left == NULL) {
            right = curr->right;
            free (curr);
            curr = right;
        } else {
            curr->left = left->right;
            left->right = curr;
            curr = left;
        }
    }

    free (S);
}
size_t set_size (const Set *S) { // get the number of elements
    return (S->size);
}
int set_includes (const Set *S, const void *data) { // check the element's presence
    Node *curr = S->root;
    int cmp;
    while (curr != NIL) {
        cmp = S->mode (data, curr->data);
        if (cmp != 0)
            curr = (cmp < 0) ? curr->left : curr->right;
        else
            return true;
    }

    return false; //return (locate (S, data) != NULL);
}
void set_insert (Set *S, const void *data) { // add an element
    Node *curr = S->root, *head = NULL;
    int cmp;
    //printf ("%s ", S->deb (data));
    while (curr != NIL) {
        cmp = S->mode (data, curr->data);

        if (cmp == 0) return;

        head = curr;
        curr = (cmp < 0) ? curr->left : curr->right;
    }

    Node *next = newnode (data);
    next->head = head;
    S->size++;

    if (head) {
        if (S->mode (data, head->data) < 0)
            head->left = next;
        else
            head->right = next;
    } else {
        S->root = next;
    }

    mk_balance (S, next);
}
void set_remove (Set *S, const void *data) { // remove an element

    Node *curr = S->root, *next, *tmp;
    int cmp;
    while (curr != NIL) {
        cmp = S->mode (data, curr->data);
        if (cmp == 0) break;
        curr = (cmp < 0) ? curr->left : curr->right;
    }

    if (!curr || curr == NIL) return;

    if (curr->left == NIL || curr->right == NIL) {
        tmp = curr;
    } else {
        tmp = curr->right;
        while (tmp->left != NIL) tmp = tmp->left;
    }
    S->size--;
    next = tmp->left != NIL ? tmp->left : tmp->right;
    next->head = tmp->head;

    if (parent (tmp)) {                  // if (tmp->head)
        if (tmp == parent (tmp)->left)   // if (tmp == tmp->head->left) {
            parent (tmp)->left = next;   //     tmp->head->left = next;
        else                             // else
            parent (tmp)->right = next;  //     tmp->head->right = next;

    } else {
        S->root = next;
    }

    if (tmp != curr)
        curr->data = tmp->data;

    if (tmp->color == BLACK && tmp != S->root)
        rebalance  (S, next);

    free (tmp);
}
