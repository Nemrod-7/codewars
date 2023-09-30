#include <numeric>
#include <algorithm>

#include "base.hpp"
#include "ntheory.hpp"
// stackoverflow d~a6fK~*>Z$~2Zy
// projecteuler NEQ5xBztxeg43aP

// -std=c++17 -Wall -Wextra -O2 -pthread -march=native

using u64 = unsigned long long int;
const u64 mod = 1000000007;

u64 phi (u64 num) { // totient funtion
    if (num == 0) return 0;
    u64 res = num;

    if (num % 2 == 0) {
        while (num % 2 == 0)
            num /= 2;

        res -= res / 2;
    }

    for (u64 pr = 3; pr * pr <= num; pr += 2) {
        if (num % pr == 0) {
            while (num % pr == 0)
                num /= pr;

            res -= res / pr;
        }
    }

    return (num > 1) ? res - res / num : res;
}
bool is_even (u64 n) { return n % 2 == 0 ? true : false; }

u64 mobius(u64 num){
    if (num == 1) return 1;
    size_t cnt = 0;

    for (u64 i = 2; i * i <= num; i++)
          if (num % i == 0) {
              num /= i;
              cnt++;

              if (num % i == 0) return 0;
          }

    return num % 2 == 0 ? -1 : 1;
}
u64 gauss (u64 n) { // gauss factorial => product of all positive numbers <= n that are relatively prime to n
  u64 mul = 1;
  // cout << "n coprimes " << phi(n) << "\n";
  for (u64 i = 2; i < n; i++) {
      if (std::gcd (i, n) == 1) {
          mul = (mul * i) % mod;
      }
  }

  return mul % mod;
}

int main () {

    Timer chrono;
    const u64 limit = 1e8;

    /*
    problem 754 - Product of Gauss Factorials
    The Gauss Factorial of a number n is defined as the product of all positive numbers <= n that are relatively prime to n.
    For example g(10) = 1 x 3 x 7 x 9 = 189.

    Also we define G(n) = \prod_{i=1}^{n} g(i)

    You are given G(10) = 23044331520000.
    Find G(10^8). Give your answer modulo 1000000007.

    φ
    1 1
    1 1
    2 2
    2 3
    4 24
    2 5
    6 720
    4 105
    6 2240
    4 189

    */

    u64 n = 10;
    u64 mul = 1;
    // cout << "n coprimes " << phi(n) << "\n";

    for (u64 i = 1; i <= n; i++) {
        // mul *= (gauss(i) % mod);
        std::cout << phi(i) << " " << gauss(i) << "\n";
    }

    // std::cout << "\ngauss :: " << mul << ' ';


    chrono.stop();
    chrono.get_duration();
}
