#include <iostream>
#include <cmath>

typedef unsigned long long ull;

ull *SieveOfEratosthenes (ull num) {
    bool *primes = new bool[num + 1];
    ull *sieve = new ull[num +1], p, i, size = 1;
    const ull end = sqrt (num);

    std::fill_n (primes, num, 0);

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
ull numPrimorial (unsigned short int n) {

    const ull *sieve = SieveOfEratosthenes (n*10);
    ull prod = 1;

    for (unsigned short int i = 1; i <= n; ++i)
        prod *= sieve[i];

    return prod;
}

int main () {

    std::cout << numPrimorial(8);
}
