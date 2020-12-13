#include <cmath>

std::vector<int> prime_factors(long n){
    std::vector<int> fact;

    for (int p = 2; p <= sqrt (n); p++)
        while (n % p == 0) {
            fact.push_back(p);
            n /= p;
        }

    if (n > 2) fact.push_back(n);

    return fact;
}
