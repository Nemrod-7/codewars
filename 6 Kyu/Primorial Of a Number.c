#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

unsigned *SieveOfEratosthenes (unsigned num) {
    bool primes[num];
    unsigned *sieve = malloc (num * sizeof (int));
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
unsigned long long num_primorial (unsigned n) {

    const unsigned *sieve = SieveOfEratosthenes (n*10);
    unsigned long long prod = 1;

    for (int i = 1; i <= n; ++i)
        prod *= sieve[i];

    return prod;
}
