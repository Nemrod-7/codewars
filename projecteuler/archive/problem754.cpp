#include <numeric>
#include <algorithm>
#include <numbers>
#include <cmath>

#include <thread>
#include <atomic>

#include "base.hpp"
#include "ntheory.hpp"

using namespace std;
// stackoverflow d~a6fK~*>Z$~2Zy
// projecteuler NEQ5xBztxeg43aP

// -std=c++20 -Wall -Wextra -O2 -pthread -march=native

u64 modpow (u64 base, u64 exp, u64 mod) {
    u64 res = 1;

    while (exp > 0) {
        if ((exp & 1) > 0) res = (res * base) % mod;
        exp >>= 1;
        if (exp > 0) base = (base * base) % mod;
    }

    return res % mod;
}
u64 PowMod(u64 x, u64 e, u64 mod) {
  u64 res;

  if (e == 0) {
    res = 1;
  } else if (e == 1) {
    res = x;
  } else {
    res = PowMod(x, e / 2, mod);
    res = res * res % mod;
    if (e % 2) res = res * x % mod;
  }

  return res;
}
u64 mulmod(u64 a, u64 b, u64 m) {
    u64 res = 0;
    u64 temp_b;

    /* Only needed if b may be >= m */
    if (b >= m) {
        if (m > UINT64_MAX / 2u)
            b -= m;
        else
            b %= m;
    }

    while (a != 0) {
        if (a & 1) {
            /* Add b to res, modulo m, without overflow */
            if (b >= m - res) /* Equiv to if (res + b >= m), without overflow */
                res -= m;
            res += b;
        }
        a >>= 1;

        /* Double b, modulo m */
        temp_b = b;
        if (b >= m - b)       /* Equiv to if (2 * b >= m), without overflow */
            temp_b -= m;
        b += temp_b;
    }
    return res;
}

i64 problem754_2 (i64 limit) {

    i64 mul = 1;
    auto phi = totient(limit);
    auto mu = moebius(limit);
    auto fact = factorial (limit);

    auto prime = sieve(limit);
    unsigned *p = prime.data();

    // 331358692
    cout << endl;
    for (i64 n = 2; n <= limit; n++) {
        i64 gauss = modpow(n, phi[n], mod);
        gauss *= fact[n] / (double) modpow(n,n,mod);
        // cout << n << " => ";

        for (int i = 0; i < prime.size() && p[i] < n; i++) {
            if (n % p[i] == 0) {
                for (int d = p[i] ; d < n; d += p[i]) {
                    if (n % d == 0) {
                        double an = fact[d] / (double) modpow(d,d,mod);// powmod(d,d, (u64) mod);
                        gauss *= pow(an, mu[n / d]);
                    }
                }
            }
        }

        cout <<  mul << ' ';
        // cout << endl;
        mul = static_cast<i64>(mul * round(gauss)) % mod  ;
    }

    return mul;
}

void gfactorial (i64 start, i64 end, std::vector<unsigned> &prime, std::atomic<i64> &res) {
    const int size = (end >> 6) + 1;
    i64 gfact = 1;
    i64 bmask[size];
    unsigned *p = prime.data();

    for (int n = start; n <= end; n++) {
        i64 gauss = 1;
        std::fill_n (bmask, (n >> 6) + 1 , 0);

        for (int i = 0; i < prime.size() && p[i] < n; i++) {
            if (n % p[i] == 0) {
                for (int j = p[i]; j < n; j += p[i]) {
                    bmask[j >> 6] |= 1UL << (j &63);
                }
            }
        }

        for (int i = 2; i < n; i++) {
            if ((bmask[i >> 6] >> (i &63) &1UL) == 0) {
                gauss = (gauss * i) % mod;
            }
        }
        gfact = (gfact * gauss) % mod;
    }

    res = (res * gfact) % mod;
}
i64 problem754_4(i64 limit) {

    const int nthread = std::thread::hardware_concurrency();

    std::thread pool[nthread];
    std::atomic<i64> res = {1};
    std::vector<unsigned> prime = sieve(limit);

    int ratio = limit / nthread;

    for (int i = 0; i < nthread; i++) {
        i64 fs  = ratio * i + 1, nd = ratio * (i + 1);
        pool[i] = std::thread (gfactorial, fs, nd, std::ref(prime), std::ref(res));
    }

    for (auto &th : pool) {
        if (th.joinable()) {
            th.join();
        }
    }

    return res;
}

int main () {

    Timer chrono;
    // constexpr auto π = std::numbers::pi;

    /*
       problem 754 - Product of Gauss Factorials
       The Gauss Factorial of a number n is defined as the product of all positive numbers <= n that are relatively prime to n.
       For example g(10) = 1 x 3 x 7 x 9 = 189.

       Also we define G(n) = \prod_{i=1}^{n} g(i)

       You are given G(10) = 23044331520000.
       Find G(10^8). Give your answer modulo 1000000007.

       i  | φ                    ∏
       ---|--------------------------
       1  | 1 :: 1            => 1
       2  | 1 :: 1            => 0
       3  | 2 :: 1 2          => 2
       4  | 2 :: 1 3          => 3
       5  | 4 :: 1 2 3 4      => 24
       6  | 2 :: 1 5          => 5
       7  | 6 :: 1 2 3 4 5 6  => 720
       8  | 4 :: 1 3 5 7      => 105
       9  | 6 :: 1 2 4 5 7 8  => 2240
       10 | 4 :: 1 3 7 9      => 189

       a(n) = (n^phi(n)) * Product_{d|n} (d! / d^d)^mu(n / d);

       problem754_1 =>
limit :        1  gauss => 1
limit :       10  gauss => 331358692
limit :      100  gauss => 777776709
limit :    1 000  gauss => 297877340
limit :   10 000  gauss => 517055464
limit :  100 000  gauss => 516871211
limit :1 000 000  gauss => 557051244

Execution time : 41 mn 5 s 50.988490 ms
*/

    int limit = 10;
    for(int i = 1; i < 1e4; i *= 10) {
      std::cout << problem754_2(i) << '\n';
    }


    // std::cout << "limit :" <<std::setw(11) << format (limit) << " gauss => ";
    // std::cout << problem754_2(limit);

    chrono.stop();
    chrono.get_duration();
}
