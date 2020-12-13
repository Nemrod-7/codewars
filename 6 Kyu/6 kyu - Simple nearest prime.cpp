#include <cmath>

inline bool is_prime (uint64_t num) {
    if (num < 4) return true;
    if (num % 2 == 0) return false;
    if (num % 3 == 0) return false;

    const uint64_t end = sqrt (num);

    for (uint64_t i = 5; i <= end; i += 6)
        if (num % i == 0 || num % (i + 2) == 0)
            return false;

    return true;
}
long long solve(long long n){
    long long m = n, p = n;

    while (true) {
        if (is_prime(m)) return m;
        if (is_prime(p)) return p;
        p++, m--;
    }
}
