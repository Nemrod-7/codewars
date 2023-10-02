#include <numeric>
#include <algorithm>
#include <numbers>

#include "base.hpp"
#include "ntheory.hpp"
// stackoverflow d~a6fK~*>Z$~2Zy
// projecteuler NEQ5xBztxeg43aP

// -std=c++20 -Wall -Wextra -O2 -pthread -march=native

using u64 = unsigned long long int;
using i64 = long long int;

const u64 mod = 1000000007;

int mu (i64 num) { // mobius function
    // μ(n) = −1 if n is a square-free positive integer with an odd number of prime factors.
    // μ(n) = +1 if n is a square-free positive integer with an even number of prime factors.
    // μ(n) =  0 if n has a squared prime factor.
    if (num == 1) return 1;
    i64 cnt = 0;

    for (i64 i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            num /= i;
            cnt++;
            if (num % i == 0) return 0;
        }
    }
    return (cnt &1ULL) == 0  ? -1 : 1;
}

i64 gauss (i64 n) { // gauss factorial => product of all positive numbers <= n that are relatively prime to n -> OEIS A001783
  i64 mul = 1, kn = 0;

  for (i64 i = 2; i < n; i++) {
      if (std::gcd (i, n) == 1) {
          mul = (mul * i) % mod;
      }
  }
  // for (i64 k = 1; kn < n; k++, kn = k / n) {
  //     if (((kn * k) % n - 1) == 0) {
  //         mul = (mul * kn) % mod;
  //     }
  // }

  return mul % mod;
}
i64 gfactorial (i64 n) {
    i64 mul = 1;
    for (i64 k = 1; k <= n; k++) {
        mul = (mul * gauss(k)) % mod;
    }
    return mul % mod;
}

void problem754 (i64 n) {
    i64 mul = 1;
  for (i64 i = 1; i <= n; i++) {
      i64 gau = 1;
      std::cout << std::setw(2) << i << " | " << phi(u64(i)) << " :: ";
      for (i64 k = 1; k < i; k++) {
          if (std::gcd(k,i) == 1) {
              gau = (gau * k) % mod;
              std::cout << k << " ";
          }
      }
      mul = (mul * gau) ;
      std::cout << " => " << gau ;
      std::cout << "\n";
  }
}

i64 factorial (i64 n) {
    i64 mult = 1;
    for (i64 i = 1; i <= n ; i++) {
        mult = (mult * i) % mod;
    }
    return mult % mod;
}
i64 exp (i64 n,i64 x) {
    i64 res = n;
    while (x-->1) res = (res * n) % mod;
    return res;
}
int main () {

    Timer chrono;
    const u64 limit = 1e8;
    // constexpr auto π = std::numbers::pi;

    /*
    problem 754 - Product of Gauss Factorials
    The Gauss Factorial of a number n is defined as the product of all positive numbers <= n that are relatively prime to n.
    For example g(10) = 1 x 3 x 7 x 9 = 189.

    Also we define G(n) = \prod_{i=1}^{n} g(i)

    You are given G(10) = 23044331520000.
    Find G(10^8). Give your answer modulo 1000000007.

    i | φ                    ∏
   ---|--------------------------
    1 | 1 :: 1            => 1
    2 | 1 :: 1            => 1
    3 | 2 :: 1 2          => 2
    4 | 2 :: 1 3          => 3
    5 | 4 :: 1 2 3 4      => 24
    6 | 2 :: 1 5          => 5
    7 | 6 :: 1 2 3 4 5 6  => 720
    8 | 4 :: 1 3 5 7      => 105
    9 | 6 :: 1 2 4 5 7 8  => 2240
   10 | 4 :: 1 3 7 9      => 189

    a(n) = (n^phi(n)) * Product_{d|n} (d! / d^d)^mu(n / d);
    */

    u64 n = 10;

    // i64 res, prod = 1;
    //
    // for (int k = 1; k < n; k++) {
    //     if (n % k == 0) {
    //         i64 f = factorial (k);
    //
    //         i64 num = exp(factorial (k) / exp (k,k), mu (n / k));
    //
    //         std::cout << factorial (k) / exp (k,k) << " ";
    //         prod = (prod * num) % mod;
    //     }
    // }
    //
    // res = exp (n, phi(n)) * prod;

    // std::cout << res;

    chrono.stop();
    chrono.get_duration();
}
