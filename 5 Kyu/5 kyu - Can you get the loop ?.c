#include <stdio.h>

typedef struct node_t Node;

struct node_t {
    int visit;
    Node* next;
};

int loop_size (Node* node) {
  Node *slow = node, *fast = node;
  int cnt = 1;

  do {
      fast = fast->next;
      if (fast) {
          fast = fast->next;
          slow = slow->next;
      }
  } while (slow != fast);

  fast = slow;
  while (fast->next != slow) {
      cnt++;
      fast = fast->next;
  }

  return cnt;
}


int main () {

  Node n1, n2, n3, n4;

  n1.next = &n2;
  n2.next = &n3;
  n3.next = &n4;
  n4.next = &n4;

  int actual = loop_size (&n1);

  printf ("%i", actual);
}

/*
typedef struct node_t Node;
struct node_t {
  Node* next;
};
int loop_size(Node*);

Test(SampleTests, FourNodesWithLoopSize3)
{
    Node n1, n2, n3, n4;

    n1.next = &n2;
    n2.next = &n3;
    n3.next = &n4;
    n4.next = &n2;

    int actual = loop_size(&n1);
    cr_assert_eq(actual, 3, "Incorrect answer for 4 nodes: tail of 1 node, and a loop of 3 nodes");
}

Test(SampleTests, NoTail)
{
    Node n1, n2, n3, n4;

    n1.next = &n2;
    n2.next = &n3;
    n3.next = &n4;
    n4.next = &n1;

    Node* startNode = &n1;
    int actual = loop_size(startNode);
    cr_assert_eq(actual, 4, "Incorrect answer for 4 nodes: no tail, and a loop of 4 nodes");
}

Test(SampleTests, TinyLoop)
{
    Node n1, n2, n3, n4;

    n1.next = &n2;
    n2.next = &n3;
    n3.next = &n4;
    n4.next = &n4;

    Node* startNode = &n1;
    int actual = loop_size(startNode);
    cr_assert_eq(actual, 1, "Incorrect answer for 4 nodes: tail of 3 nodes, and a tiny loop of 1 nodes");
}

Test(SampleTests, SingleNode) {
  Node n;
  n.next = &n;
  int actual = loop_size(&n);
  cr_assert_eq(actual, 1, "Incorrect answer for a loop with a single node pointing to itself");
}
*/
