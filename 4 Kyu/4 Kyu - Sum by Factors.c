#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define NUM *(list + size)
#define BUFFSIZE 1024
#define MAX(x,y) (((x) > (y)) ? (x) : (y))

unsigned *SieveOfEratosthenes (unsigned num) {
    bool primes[num];
    unsigned *sieve = malloc (num * sizeof (int));
    const unsigned end = sqrt (num);
    register int p, i, size = 1;

    memset (primes, false, num);

    for (p = 2; p <= end ; p++)
        if (primes[p] == false)
            for (i = p * p; i <= num; i += p)
                 primes[i] = true;

    for (i = 2; i <= num; i++)
         if (primes[i] == false)
            sieve[size++] = i;

    sieve[0] = size;
    return sieve;
}
unsigned maxnum (int* list,int size) {

    unsigned max = 0;
    while (size-->0)
        max = MAX (max, abs (NUM));

    return max;
}
char *sumOfDivided (int *list, int l) {

    int sum = 0, max = maxnum (list,l) + 1;
    char *output = malloc (BUFFSIZE * sizeof(char)), *out = output;
    unsigned *primes = SieveOfEratosthenes (max);
    bool divided = false;
    *output = '\0';

    for (int i = 1; i < primes[0]; ++i) {
        for (int size = 0; size < l; ++size)
            if (abs (NUM) % primes[i] == 0)
                sum += NUM, divided = true;

        if (divided == true)
            out += sprintf (out,"(%i %i)", primes[i], sum);

        sum = 0, divided = false;
    }
    return output;
}
