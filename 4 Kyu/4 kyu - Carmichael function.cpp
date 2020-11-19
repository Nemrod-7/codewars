#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

bool ispowof2 (size_t n) { return n != 0 && (n & (n-1)) == 0; }
size_t lcm (size_t a, size_t b) { return a * b / __gcd (a, b); }
bool is_coprime (size_t a, size_t b) { return __gcd(a, b) == 1;}

vector<size_t> SieveOfEratosthenes (size_t num) {

    size_t p, i;
    const size_t end = sqrt (num);
    vector<bool> primes (num);
    vector<size_t> Sieve;

    for (p = 2; p <= end ; p++)
        if (primes[p] == false)
            for (i = p * p; i <= num; i += p)
                 primes[i] = true;

    for (i = 2; i <= num; i++)
        if (primes[i] == false)
            Sieve.push_back (i);

    return Sieve;
}
size_t phy (size_t n){
    size_t res = n;

    for (auto &prime : SieveOfEratosthenes (sqrt (n)))
        if (n % prime == 0) {
            while (n % prime == 0)
                n /= prime;

            res -= res / prime;
        }

    return (n > 1) ? res - res / n : res;
}
bool is_primepow (size_t num) {
    if (num <= 3) return true;
    if (num % 2 == 0) return false;

    for (size_t i = 3; i <= sqrt (num); i++) {
        size_t p = i;
        while (p < num) {
            p *= i;
            if (p == num) return true;
        }
        if (num % i == 0) return false;
    }

    return true;
}

long long Carmichael (long long num) {
    if (num < 1) return 0;;
    if (num < 8 || is_primepow (num)) return phy (num);
    if (ispowof2 (num)) return phy (num) / 2;

    size_t idx = ceil (sqrt (num)) ;

    while (idx-->2)
        if (num % idx == 0 && is_coprime (idx , num / idx))
            return lcm (Carmichael (idx), Carmichael (num / idx));

    return 0;
}
