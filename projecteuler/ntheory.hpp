#pragma once

#include <string>
#include <vector>
#include <cmath>
#include <cstdint>

using u64 = unsigned long int;
using i64 = long int;

const i64 mod = 1000000007;

uint64_t gcd (uint64_t a, uint64_t b) {
    while (b) b ^= a ^= b ^= a %= b;
    return a;
}
i64 lcm (i64 a, i64 b) { return a * gcd (a,b) / b; }

bool issquare (i64 num) {
    i64 sq = sqrt(num);
    return sq * sq == num;
}
bool is_prime (i64 num) {

    if (num < 2) return false;
    if (num < 4) return true;
    if (num % 2 == 0 || num % 3 == 0 ) return false;

    for (i64 i = 5; i * i <= num; i += 6)
        if (num % i == 0 || num % (i + 2) == 0)
            return false;

    return true;
}
std::vector<uint32_t> sieve (i64 limit) { // SOE with wheel factorization => ex limit == 1e8 : memory usage ~31.71 MB / execution time ~0.80ms
    const uint64_t hal = ((limit / 3) >> 6) ; // divide limit by 192
    uint64_t *sieve = new uint64_t[hal + 1]();
    std::vector<uint32_t> vs {2,3};

    for (uint64_t i = 5, t = 2 ; i * i <= limit; i += t, t = 6 - t) { // wheel factorization : 2,4A
        uint64_t p = 0xAAAAAAABULL * i >> 33;           // fast division by 3
        uint64_t mask = sieve[p >> 6] >> (p &63) &1ULL; // x >> 6 => fast division by 64 / x &63 => fast modulus 64

        if (mask == 0) {
            for (uint64_t j = i * i, v = t; j <= limit; j += v * i, v = 6 - v) {
                uint64_t p2 = 0xAAAAAAABULL * j >> 33;
                sieve[p2 >> 6] |= 1ULL << (p2 &63);
            }
        }
    }

    for (uint32_t i = 5, t = 2; i <= limit; i += t, t = 6 - t) {
        uint32_t p = 0xAAAAAAABULL * i >> 33;

        if ((sieve[p >> 6] >> (p &63) &1ULL) == false) {
            vs.push_back(i);
        }
    }

    delete[] sieve;
    return vs;
}

i64 tau (i64 n) { // count number of divisors
    i64 total = 1;

    for (; (n & 1) == 0; n >>= 1) // Deal with powers of 2 first
        ++total;

    for (i64 p = 3; p * p <= n; p += 2) { // Odd prime factors up to the square root
        int count = 1;
        for (; n % p == 0; n /= p)
            ++count;
        total *= count;
    }

    if (n > 1) total *= 2; // If n > 1 then it's prime
    return total;
}
i64 sigma (i64 num) { // sum of proper divisors

    i64 n = num, sum = 1;
    i64 p = 2;

    while (p * p <= n && n > 1) {
        if (n % p == 0) {
            i64 j = p * p;
            n /= p;

            while (n % p == 0) {
                j *= p;
                n /= p;
            }

            sum = sum * (j - 1) / (p - 1);
        }
        p += (p == 2) ? 1 : 2;
    }

    if (n > 1) sum *= (n + 1);

    return sum - num;
}

i64 phi (i64 num) { // totient funtion

    i64 res = num;

    if (num % 2 == 0) {
        while (num % 2 == 0)
            num /= 2;

        res -= res / 2;
    }

    for (i64 pr = 3; pr * pr <= num; pr += 2) {
        if (num % pr == 0) {
            while (num % pr == 0)
                num /= pr;

            res -= res / pr;
        }
    }

    return (num > 1) ? res - res / num : res;

}
i64 phi2 (i64 num, std::vector<i64> &prime) { // totient funtion

    i64 res = num;
    i64 *p = prime.data();

    for (i64 i = 0; p[i] * p[i] <= num; i++) {
        if (num % p[i] == 0) {
            while (num % p[i] == 0)
                num /= p[i];

            res -= res / p[i];
        }
    }

    return (num > 1) ? res - res / num : res;
}
std::vector<i64> totient (i64 lim) { // sieve of totient function
    std::vector<i64> sieve (lim + 1);

    for (i64 i = 0; i <= lim; i++)
        sieve[i] = i;

    for (i64 i = 2; i <= lim; i++) {
        if (sieve[i] == i) {
            for (i64 j = i; j <= lim; j += i)
                sieve[j] -= sieve[j] / i;
        }
    }
    return sieve;
}
std::vector<short> moebius (i64 limit) { // sieve of mobius function

    i64 max = limit + 1;
    std::vector<short> sieve (max);
    sieve[1] = 1;
    // μ(n) = −1 if n is a square-free positive integer with an odd number of prime factors.
    // μ(n) = +1 if n is a square-free positive integer with an even number of prime factors.
    // μ(n) =  0 if n has a squared prime factor.

    for(i64 i = 2; i < max; i++) {
        if (sieve[i]) continue;

        for(i64 j = i; j < max; j += i)
            sieve[j]++;
    }

    for(i64 i = 2; i * i < max; i++) {
        if(sieve[i] != 1) continue;
        i64 sqi = i * i;
        for (i64 j = sqi; j < max; j += sqi)
            sieve[j] = 0;
    }

    for(i64 i = 2; i < max; i++) {
        if(!sieve[i]) continue;

        sieve[i] = (sieve[i]&1) ? -1 : 1;
    }

    return sieve;
}

i64 fibonacci (i64 n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    return fibonacci (n - 1) + fibonacci (n - 2);
}
std::string collatz (i64 n) {

    std::string os;

    while (n != 1) {
        os += std::to_string (n) + " -> ";
        n = (n % 2 == 0) ? n * 0.5 : 3 * n + 1;
    }
    return os + "1";
}
void collatz2 (i64 a1) {
    const char alpha[3] = {'D','U','d'};
    std::string seq;
    // std::cout << a1 << " => ";
    while (a1 > 1) {
        int mod = a1 % 3;

        switch (mod) {
            case 0 : a1 = a1 / 3; break;
            case 1 : a1 = (4 * a1 + 2) / 3; break;
            case 2 : a1 = (2 * a1 - 1) / 3; break;
            default: break;
        }
        seq += alpha[mod];
        // std::cout << alpha[mod];
    }
}

bool isPentagonal (i64 N) {
    double n = (1 + sqrt(24*N + 1))/6;
    return (n - (i64) n) == 0;
}
bool check_goldbach (i64 num, const std::vector<i64> &prime) {

    for (i64 i = 0; i < prime.size() && prime[i] < num; i++) {
        for (i64 k = 1; k * k < num; k++) {
            if (prime[i] + 2 * (k * k) == num) {
                //cout << prime[i] << " + 2 x " << k << "²";
                return true;
            }
        }
    }

    return false;
}
void farey (int n) {

    typedef struct { int d, n; } frac;
    frac f1 = {0, 1}, f2 = {1, n}, tmp;
    int k;
    printf("%d/%d ", 1, n);
    while (f2.n > 1) {
        k = (n + f1.n) / f2.n;
        tmp = f1;
        f1 = f2;
        f2 = (frac) { f2.d * k - tmp.d, f2.n * k - tmp.n };
        // std::cout << f2.d << "/" << f2.n << " ";
    }
}
i64 cntdiv (i64 num) {
    int np = 0;

    while ((num &1) == 0) {
        num >>= 1;
        np++;
        if (np > 2) return np;
    }

    for (i64 p = 3; p * p <= num; p += 2) {
        while (num % p == 0) {
            num /= p;
            np++;
            if (np > 2) return np;
        }
    }
    if (num > 1) np++;
    return np;
}

i64 reverse (i64 num) {

    i64 rev = 0;

    do {
        rev = rev * 10 + num % 10;
    } while (num /= 10);

    return rev;
}
bool palindrome (i64 num) {
    if (num % 10 == 0) return false;
    return reverse (num) == num;
}

i64 gaussian (i64 n) { // gauss factorial => product of all positive numbers <= n that are relatively prime to n -> OEIS A001783
    auto prime = sieve(n);
    const int size = (n >> 6) + 1;
    i64 mul = 1, bmask[n];

    std::fill_n (bmask, size, 0);

    for (int i = 0; i < prime.size() && prime[i] < n; i++) {
        if (n % prime[i] == 0) {
            for (int j = prime[i]; j < n; j += prime[i]) {
                bmask[j >> 6] |= 1UL << (j &63);
            }
        }
    }

    for (int i = 2; i < n; i++) {
        if ((bmask[i >> 6] >> (i &63) &1UL) == 0) {
            mul = (mul * i) % mod;
        }
    }

    return mul;
}

i64 radical (i64 n) {
    // A007947 -> r(n) -> Π p|n
    if (is_prime (n)) return n;

    i64 res = (n % 2 == 0) ? 2 : 1;
    while (n % 2 == 0) n /= 2;

    for (i64 i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            res *= i;
            while (n % i == 0) n /= i;
        }
    }

    if (n != 1) res *= n;
    return res;
}
double resilience (i64 d) { return phi (d) / static_cast<double> (d-1); }

std::vector<i64> factorial (i64 limit) {
    std::vector<i64> fact (limit + 1);
    fact[0] = 1;

    for (int i = 1; i <= limit; i++) {
        fact[i] = (fact[i-1] * i) % mod;
    }

    return fact;
}
std::vector<std::vector<uint64_t>> triplet (uint64_t limit) {
    uint64_t n = 1, m = 2;
    uint64_t a, b, c;
    std::vector<std::vector<uint64_t>> tri;

    while ((m * m + 1) < limit) {
        if (n >= m) n = m % 2, m = m + 1;
        c = m * m + n * n;

        if (c >= limit) {
            n = m;
        } else {
            if (gcd(m,n) == 1) {
                a = m * m - n * n;
                b = 2 * m * n;
                // cout << a << " " << b << " " << c << "\n" ;
                tri.push_back({a,b,c});
            }
        }
        n += 2;
    }

    return tri;
}
