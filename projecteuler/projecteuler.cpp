#include "base.hpp"

#include <thread>
#include <atomic>
// stackoverflow d~a6fK~*>Z$~2Zy
// projecteuler NEQ5xBztxeg43aP
using namespace std;

// -std=c++17 -Wall -Wextra -O2 -pthread -march=native

void search (const vector<uint64_t> &prime, const bool *sieve, int low, int high, atomic<int64_t> &res) {
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
      pool[i] = thread (search, ref (prime), ref (sieve), low, high, ref(sum));
  }

  for (int i = 0; i < nthread; i++)
      pool[i].join();

  cout << " => " << sum;

  delete[] sieve;
}

int main () {

    Timer chrono;

    // Problem 123 Prime square remainder
    // problem 120 Square Remainder
    // problem510 (100);




    chrono.stop();
    chrono.get_duration();
}
