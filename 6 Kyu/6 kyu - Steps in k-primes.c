#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct node {
    int data;
    struct node *next;
} Node;

typedef struct {
    int first, snd;
} Pair;

Pair **createpair (int size) {

    Pair **new = malloc (size * sizeof (Pair));

    for (int i = 0; i < size; ++i)
        new[i] = malloc (sizeof (Pair));

    return new;
}

unsigned *SieveOfEratosthenes (unsigned num) { // 179 passed without sieve
    bool primes[num];
    unsigned *sieve = malloc (num * sizeof (unsigned));
    const unsigned end = sqrt (num);
    register int p, i, size = 1;

    memset (primes, true, num);

    for (p = 2; p <= end ; p++)
        if (primes[p] == true)
            for (i = p * p; i <= num; i += p)
                 primes[i] = false;


    for (i = 2; i <= num; i++)
         if (primes[i] == true)
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
//////////////////////////////Testng///////////////////////////////////////////
Pair** kprimesStep(int k, int step, int start, int nd, int* length);
// sz number of pairs / Pair** arr will be freed
char* array2StringData(Pair** arr, int sz);

void dotest(int k, int step, int start, int nd, char* sexpr) {
    int lg = 0;
    Pair** act = kprimesStep(k, step, start, nd, &lg);
    char* sact = array2StringData(act, lg);
    printf("k: %d: step: %d start: %d end: %d\nExpected: %s\n", k, step, start, nd, sexpr);
    if(strcmp(sact, sexpr) != 0)
        printf("Error. Expected %s but got %s\n", sexpr, sact);
    //else printf("...............GOOD\n");
    //cr_assert_str_eq(sact, sexpr, "");
    if (strcmp(sact, "{}") != 0) {
        free(sact); sact = NULL;
    }
}

int countPrimeFactorsCCD(int n) {
    int cnt = 0;
    for (int i = 2; i * i <= n; i++)
        while (n % i == 0) {
            cnt++;
            n /= i;
        }
    if (n > 1) cnt++;
    return cnt;
}
Pair** kprimesStepCCD(int k, int step, int start, int nd, int* length) {
    Pair** result = calloc(0, sizeof(Pair*));
    int cnt = 0;
    for (int i = start; i <= nd - step; i++) {
        if (countPrimeFactorsCCD(i) == k && countPrimeFactorsCCD(i + step) == k) {
            Pair* pr = calloc(1, sizeof(Pair));
            pr->first = i;
            pr->snd = i + step;
            result = (Pair**)realloc(result, (cnt + 1) * sizeof(Pair*));
            result[cnt++] = pr;
        }
    }
    *length = cnt;
    return result;
}

int randomGen(int a, int b) {
    int r = rand();
    return r % (b - a) + a;
}
void randomtesting() {
    printf("\n****************** Random Tests **** kprimesStep\n");
    srand(time(NULL));

    for (int i = 0; i < 200; i++) {
        int k = randomGen(2, 8);
        int step = randomGen(5, 10);
        int m = randomGen(200000, 300000);
        int n = m + randomGen(200, 1000);
        int lg = 0;
        Pair** sol = kprimesStepCCD(k, step, m, n, &lg);
        char* ssol = array2StringData(sol, lg);

        dotest(k, step, m, n, ssol);

        if (strcmp(ssol, "{}") != 0) {
            free(ssol); ssol = NULL;
        }
    }
}
//////////////////////////////////////////////////////////////////////////////
int main () {

}
