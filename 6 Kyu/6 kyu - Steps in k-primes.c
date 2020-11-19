#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct node {
    int data;
    struct node *next;
} Node;

typedef struct {
    int first;
    int snd;
} Pair;

Pair **createpair (int size) {

    Pair **new = malloc (size * sizeof (Pair));

    for (int i = 0; i < size; ++i)
        new[i] = malloc (sizeof (Pair));

    return new;
}

unsigned *SieveOfEratosthenes (unsigned num) { // 179 passed without sieve
    bool primes[num];
    int *sieve = malloc (num * sizeof (int));
    const unsigned end = sqrt (num);
    register int p, i, size = 1;

    memset (primes, false, num);

    for (p = 2; p <= end ; p++)
        if ( primes[p] == false)
            for (i = p * p; i <= num; i += p)
                 primes[i] = true;


    for (i = 2; i <= num; i++)
         if (primes[i] == false)
            sieve[size++] = i;

    sieve[0] = size;
    return sieve;
}

Node *newnode (Node *cell, int data) {

    if (cell == NULL) {
        cell = malloc (sizeof (struct node));
        cell->data = data;
        cell->next = NULL;
    }
    else
        cell->next = newnode (cell->next,data);

    return cell;
}
unsigned count_k(int num, const unsigned *sieve) {
    int i = 1, count = 0;
    unsigned size = sieve[0];
    int div;

    while (num > 1) {
        div = *(sieve + i);
        while (num % div == 0) {
            num /= div;
            count++;
        }
        i++;
    }

    return count;
}
Node *kPrimes(int k, int start, int end) {
    unsigned i, num , size = 1;
    Node *stack = NULL;
    const unsigned *primes = SieveOfEratosthenes (end);

    for (num = start; num <= end; ++num)
         if (count_k (num,primes) == k)
            stack = newnode (stack,num);

    return stack;
}

Pair** kprimesStep(int k, int step, int start, int end, int *lg) {
    unsigned size = (end - start);
    printf ("\n k %i stp %i strt %i end %i\n",k,step,start,end);
    Node *root = kPrimes(k, start, end), *fst, *scd;
    Pair **result = createpair (size);

    size = 0;

    for (fst = root; fst; fst = fst->next)
        for (scd = root; scd; scd = scd->next)
            if (scd->data - fst->data == step) {
                result[size]->first = fst->data;
                result[size]->snd = scd->data;
                size++;
            }

    *lg = size;
    return result;
}
