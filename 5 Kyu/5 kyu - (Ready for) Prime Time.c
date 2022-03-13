#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void prime (unsigned num, size_t *z, unsigned *sieve) {
    unsigned p, i, end = sqrt (num);
    _Bool primes[num + 1];
    
    memset (primes, 0, num + 1);
    
    for (p = 2; p <= end ; p++)
      if (primes[p] == 0)
          for (i = p * p; i <= num; i += p)
               primes[i] = 1;
  
    for (p = 0, i = 2; i <= num; i++)
      if (primes[i] == 0) {
          sieve[p++] = i;
      }
  
    *z = p;
}
