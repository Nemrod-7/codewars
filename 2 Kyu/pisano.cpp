#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cstdint>
#include "base.hpp"

// 2 kyu - Pisano Period - Performance Edition
using namespace std;

uint64_t addmod (uint64_t a, uint64_t b, uint64_t mod ) {
  a %= mod, b %= mod;
  int64_t sum = a - mod + b; // -mod <= sum < mod-1
  return sum < 0 ? sum + mod : sum;
}
uint64_t mulmod (uint64_t a, uint64_t b, uint64_t mod) {
    uint64_t res = 0;

    while (b > 0) {
        if (b & 1) {
            res = (res + a) % mod;
        }
        a = (a * 2) % mod;
        b >>= 1;
    }

    return res;
}
uint64_t powmod (uint64_t a, uint64_t b, uint64_t mod) {
    uint64_t res = 1;
    a %= mod;

    while (b > 0) {
        if (b & 1) {
            res = mulmod(res, a, mod);
        }
        a = mulmod(a, a, mod);
        b >>= 1;
    }

    return res;
}

uint64_t gcd (uint64_t a, uint64_t b) {
    while (b) b ^= a ^= b ^= a %= b;
    return a;
}
uint64_t lcm (uint64_t a, uint64_t b) {
    return (a / gcd (a,b)) * b;
}

vector<pair<uint64_t,uint64_t>> factors (uint64_t N) { // returns all factors of N -> [prime, exponent]
    vector<pair<uint64_t,uint64_t>> vs;
    vector<int> pr = {2,3,5,7};
    const int wheel[48] = {2,4,2,4,6,2,6,4,2,4,6,6,2,6,4,2,6,4,6,8,4,2,4,2,4,8,6,4,6,2,4,6,2,6,6,4,2,4,6,2,6,4,2,4,2,10,2,10};

    for (auto &i : pr) {
        if (N % i == 0) {
            int ex = 0;
            while (N % i == 0) {
                N /= i, ex++;
            }
            vs.push_back({i,ex});
        }
    }

    for (uint64_t i = 11, t = 0; i * i <= N; i += wheel[t], t = t == 47 ? 0 : t + 1) {
        if (N % i == 0) {
            int ex = 0;
            while (N % i == 0) {
                N /= i, ex++;
            }
            vs.push_back({i,ex});
        }
    }

    if (N > 1) vs.push_back({N,1});
    return vs;
}
vector<uint64_t> divisors (const uint64_t N) { // returns all sorted divisors of N
    vector<uint64_t> divisor = {1};
    vector<pair<uint64_t,uint64_t>> fac = factors(N);

    for (auto &[pr,ex] : fac) {
        int mult = 1, end = divisor.size();

        for (int j = 0; j < ex; j++) {
            mult *= pr;

            for (int k = 0; k < end; k++) {
                divisor.push_back(divisor[k] * mult);
            }
        }
    }

    sort(divisor.begin(), divisor.end());
    return divisor;
}

pair<uint64_t,uint64_t> fibonacci (uint64_t n, uint64_t mod) {
  // Function calculate the N-th fibonacci number using fast doubling method => returns [Fn, Fn+1]
    if (n == 0) return {0,1};
    auto [f0, f1] = fibonacci (n / 2, mod);
    // As F(2n + 1) = F(n)^2 + F(n+1)^2, As F(2.n) = F(n)[2.F(n+1) – F(n)]
    int64_t c = mulmod(2, f1, mod) - f0, d = addmod(f0 * f0, f1 * f1, mod);
    cout << d << " ";
    if (c < 0) c += mod;
    c = mulmod(f0, c, mod);

    return (n % 2 == 0) ? pair<uint64_t,uint64_t> {c, d} : pair<uint64_t,uint64_t> {d, c + d};
}

uint64_t pisano_prime (uint64_t N) {

    const pair<uint64_t, uint64_t> period = {0,1};
    uint64_t cycle = 2 * N + 2;
    vector<uint64_t> divisor, d1 = divisors(N - 1);

    if ((N - 1) % 10 == 0 || (N + 1) % 10 == 0) {
        for (auto &d : d1) {
            if (d % 2 == 0) {
                divisor.push_back(d);
            }
        }

    } else {
        vector<uint64_t> d2 = divisors(2 * N + 2);

        for (auto &d : d2) {
            if (find(d1.begin(), d1.end(), d) == d1.end()) {
                divisor.push_back(d);
            }
        }
    }

    for (auto &d : divisor) {
        // cout << fibonacci(d, N) << "\n" ;
        if (fibonacci(d, N) == period) {
            cycle = min(d,cycle);
        }
    }

    return cycle;
}
uint64_t pisano_period (uint64_t N) { // OEIS A001175 pisano period of fib(i) mod n
    if (N == 1) return 1;
    if (N == 2) return 3;
    if (N == 5) return 20;

    uint64_t cycle = 1;
    vector<pair<uint64_t,uint64_t>> fac = factors(N);

    if (fac.size() == 1 && fac[0].second == 1) { // if n is a prime number
        cycle = pisano_prime(N);
    } else {
        // if p is a prime number and k is interger power : pisano(p^k) == p^(k-1) * pisano (p)
        for (auto &[p,ex] : fac) {
            cycle = lcm(cycle, pisano_period(p) * powmod(p, ex - 1, N)) ;
        }
    }

    return cycle;
}

int main () {

    const vector<int> pi = {1, 1, 3, 8, 6, 20, 24, 16, 12, 24, 60, 10, 24, 28, 48, 40, 24, 36, 24, 18, 60, 16, 30, 48, 24, 100, 84, 72, 48, 14, 120, 30, 48, 40, 36, 80, 24, 76, 18, 56, 60, 40, 48, 88, 30, 120, 48, 32, 24, 112, 300, 72, 84, 108, 72, 20, 48, 72, 42, 58, 120, 60, 30, 48, 96, 140, 120, 136};

    int64_t N = 2474324059;
    int64_t d = 2474324058;

    // Assert::That(pisano_period(4256513951), Equals(851302790)); // primes_32_bits
    // Assert::That(pisano_period(1840094077), Equals(3219724224703183324)); // primes_64_bits

    fibonacci(851302790, 4256513951);

    // Test();
    cout << "\nexit\n";
    //Assert::That(pisano_period(2438389198053), Equals(10624179384));
}

/*
uint64_t pisano_period2 (uint64_t N) { // OEIS A001175 pisano period of fib(i) mod n
    if (N == 1) return 1;
    if (N == 2) return 3;
    if (N == 5) return 20;

    const pair<int64_t, int64_t> period = {0,1};
    uint64_t cycle = 1;
    vector<pair<uint64_t,uint64_t>> fac = factors(N);

    if (fac.size() == 1 && fac[0].second == 1) { // if n is a prime number
        vector<uint64_t> divisor, d1 = divisors(N - 1);
        cycle = 2 * N + 2;

        if ((N - 1) % 10 == 0 || (N + 1) % 10 == 0) {
            for (auto &d : d1) {
                if (d % 2 == 0) {
                    divisor.push_back(d);
                }
            }

        } else {
            vector<uint64_t> d2 = divisors(2 * N + 2);

            for (auto &d : d2) {
                if (find(d1.begin(), d1.end(), d) == d1.end()) {
                    divisor.push_back(d);
                }
            }
        }

        for (auto &d : divisor) {

            if (fibonacci(d, N) == period) {
                cycle = min(d,cycle);
            }
        }
    } else {
        // if p is a prime number and k is interger power : pisano(p^k) == p^(k-1) * pisano (p)
        for (auto &[p,ex] : fac) {
            cycle = lcm(cycle, pisano_period(p) * powmod(p, ex - 1, N)) ;
        }
    }

    return cycle;
}
*/
