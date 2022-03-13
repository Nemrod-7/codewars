#include <vector>
#include <cmath>

std::vector<unsigned> prime (unsigned num){
    std::vector<bool> primes (num);
    unsigned p, i, end = sqrt (num);
    std::vector<unsigned> sieve;

    for (p = 2; p <= end ; p++)
        if (primes[p] == false)
            for (i = p * p; i <= num; i += p)
                 primes[i] = true;
  
    for (i = 2; i <= num; i++)
        if (primes[i] == false) {
            sieve.push_back (i);
        }

    return sieve;
}
