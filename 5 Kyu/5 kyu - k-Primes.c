#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


struct node {
    int data;
    struct node *next;
};
struct list {
    size_t sz;
    struct node *head;
};

struct list* createList();


void insertFirst(struct list* l, int data);

struct list* reverse(struct list* l);
void listFree(struct list* l);

size_t count_k(int num) {
    size_t i, count = 0;

    while (num > 1) {
        for (int i = 2; i <= num; ++i)
                if (num % i == 0) {
                    num /= i;
                    break;
                }
        count++;
    }

    return count;
}
struct list* kPrimes(int k, int start, int end) {
    size_t i, num , size = 1;
    //struct list *stack = createList();

    for (num = start; num <= end; ++num)
         (count_k (num) == k);
            //insertFirst (stack,num);

    //stack = reverse (stack);
    //return stack;
}
int puzzle(int s) {
    unsigned cnt = 0;

    struct list* c = kPrimes(7, 0, s);
    struct list* b = kPrimes(3, 0, s);
    struct list* a = kPrimes(1, 0, s);
    struct node* seven, *three, *one;

    for (seven = c->head; seven; seven = seven->next)
        for (three = b->head; three; three = three->next)
            for (one = a->head; one; one = one->next)
                if (seven->data + three->data + one->data == s) {

                    cnt++;
                }
    return cnt;
}
int main () {

  kPrimes(2,0,100);
  kPrimes(3,0,100);
  kPrimes(5,1000,1100);
  kPrimes(5,500,600);
  kPrimes(7,1000,1500);
  kPrimes(7,10000,10100);
  kPrimes(7,100000,100100);
  kPrimes(12,100000,100100);
  kPrimes(1,2,30);
  kPrimes(8,10000000,10000200);
  puzzle(138);
  kPrimes(7,0,138);
  kPrimes(3,0,138);
  kPrimes(1,0,138);
  puzzle(143);
  kPrimes(7,0,143);
  kPrimes(3,0,143);
  kPrimes(1,0,143);
  puzzle(250);
  kPrimes(7,0,250);
  kPrimes(3,0,250);
  kPrimes(1,0,250);
  puzzle(2500);
  kPrimes(7,0,2500);
  kPrimes(3,0,2500);
  kPrimes(1,0,2500);
  puzzle(2600);
  kPrimes(7,0,2600);
  kPrimes(3,0,2600);
  kPrimes(8,5381330,5383080);
  kPrimes(7,5722535,5724490);
  kPrimes(8,2698220,2699701);
  kPrimes(6,6801228,6802637);
  kPrimes(7,2720454,2721961);
  kPrimes(7,5524555,5526024);
  kPrimes(8,5050029,5051958);
  kPrimes(8,2455414,2457057);
  kPrimes(6,7605724,7607569);
  kPrimes(6,4522635,4523668);
  kPrimes(7,3148415,3149508);
  kPrimes(5,2328347,2330286);



  printf ("df");


}
