#include "base.hpp"
#include <algorithm>

#include <thread>
#include <atomic>

using namespace std;

// stackoverflow d~a6fK~*>Z$~2Zy
// projecteuler NEQ5xBztxeg43aP

// -std=c++17 -Wall -Wextra -O2 -pthread -march=native

void searchsieve (const vector<uint64_t> &prime, const bool *sieve, int low, int high, atomic<int64_t> &res) {
    const int limit = prime.back() + 2;
    const int size = prime.size();
    const uint64_t *p = prime.data();

    for (int i = low; i < high; i++) {
        uint64_t a = p[i] + 1;

        for (int j = i + 1; j < size; j++) {
            uint64_t b = p[j] + 1;
            uint64_t b2 = b * b;
            uint64_t c = b2 / a;

            if (c > limit) break;
            if (b2 % a != 0) continue;

            if (sieve[c - 1] == false) {
                // cout << "(" << a -1 << " " << b - 1 << " " << c - 1 << ")" ;
                res += a + b + c - 3;
            }
        }
    }
}
void problem518 () {
  // problem 518
  // vector<vector<int>> seq = {{2, 5, 11}, {2, 11, 47}, {5, 11, 23}, {5, 17, 53}, {7, 11, 17}, {7, 23, 71}, {11, 23, 47}, {17, 23, 31}, {17, 41, 97}, {31, 47, 71}, {71, 83, 97}}; // => 1035
  const int64_t limit = 1e6;
  bool *sieve = new bool[limit]();
  vector<uint64_t> prime {2,3};

  for (uint64_t i = 4; i < limit; i += 2) {
      sieve[i] = true;
  }

  for (uint64_t i = 3; i * i < limit; i += 2) {
      if (sieve[i] == false) {
          for (uint64_t j = i * i; j < limit; j += i) {
              sieve[j] = true;
          }
      }
  }

  for (uint64_t i = 5, t = 2; i < limit; i += t, t = 6 - t) {
      if (sieve[i] == false) {
          prime.push_back(i);
      }
  }

  const int nthread = thread::hardware_concurrency();
  const int64_t mem = prime.size() / nthread;
  thread pool[nthread];
  atomic<int64_t> sum = {0};

  for (int i = 0; i < nthread; i++) {
      int low = i * mem, high = low + mem ;
      pool[i] = thread (searchsieve, ref (prime), ref (sieve), low, high, ref(sum));
  }

  for (int i = 0; i < nthread; i++)
      pool[i].join();

  cout << " => " << sum;

  delete[] sieve;
}

int64_t phi (int64_t num) { // totient funtion
    if (num == 0) return 0;
    int64_t res = num;

    if (num % 2 == 0) {
        while (num % 2 == 0)
            num /= 2;

        res -= res / 2;
    }

    for (int64_t pr = 3; pr * pr <= num; pr += 2) {
        if (num % pr == 0) {
            while (num % pr == 0)
                num /= pr;

            res -= res / pr;
        }
    }

    return (num > 1) ? res - res / num : res;
}
bool is_even (uint64_t n) { return n % 2 == 0 ? true : false; }

int mobius(uint64_t num){
    if (num == 1) return 1;
    size_t cnt = 0;

    for (uint64_t i = 2; i * i <= num; i++)
          if (num % i == 0) {
              num /= i;
              cnt++;

              if (num % i == 0) return 0;
          }

    return is_even(cnt) ? -1 : 1;
}
int gauss (int n) { // gauss factorial => product of all positive numbers <= n that are relatively prime to n
  int mul = 1;
  // cout << "n coprimes " << phi(n) << "\n";
  for (int i = 0; i < n; i++) {
      if (__gcd (i, n) == 1) {
          mul *= i;
      }
  }

  return mul;
}
int main () {

    Timer chrono;

    // Problem 123 Prime square remainder
    // problem 120 Square Remainder
    // problem510 (100);

    /*

    problem 754 - Product of Gauss Factorials

    The Gauss Factorial of a number n is defined as the product of all positive numbers <= n that are relatively prime to n.
    For example g(10) = 1 x 3 x 7 x 9 = 189.

    Also we define
    G(n) = \prod_{i=1}^{n}g(i)

    You are given G(10) = 23044331520000.

    Find G(10^8). Give your answer modulo 1 000 000 007.

    */
    int n = 10;
    int g = 1;

    // cout << "n coprimes " << phi(n) << "\n";
    //
    // for (int i = 0; i < n; i++) {
    //     if (__gcd (i, n) == 1) {
    //         cout << i << " ";
    //         g *= i;
    //     }
    // }
    //
    // cout << g << ' ';


    chrono.stop();
    chrono.get_duration();
}
