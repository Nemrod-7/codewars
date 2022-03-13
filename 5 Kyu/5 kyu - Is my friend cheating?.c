#include <stdlib.h>

typedef struct _pair { long long first, snd; } Pair;

Pair *mkpair (long long a, long long b) {
    Pair *p = malloc (sizeof(Pair));
    p->first = a, p->snd = b;
    return p;
}
Pair** removNb(long long n, int* length) {
    long long cnt = n * (n + 1) / 2, a = n, b, sum;
    Pair **seq = malloc (5 * sizeof(Pair));
    int index = 0;
    
    while (a-->2) {
        b = (cnt - a) / (a + 1), sum = cnt - a - b;
        if (b < n && a * b == sum) seq[index++] = mkpair (b,a);
    }
  
    *length = index; 
    return seq;
}
