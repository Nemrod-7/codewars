#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>

#include <algorithm>

#include <thread>
#include <atomic>
#include <chrono>

using namespace std;

void trilet (uint64_t limit) {
  uint64_t a, b, c = 0;

  for (uint64_t m = 2; (m * m + 1) < limit; m++) {
      for (uint64_t n = 1; n < m ; n++) {
          if ((m + n) % 2 == 0 || __gcd (m,n) != 1) continue; // primitive right triangle : cannot be scaled to another smaller right triangle

          a = m * m - n * n;
          b = 2 * m * n;
          c = m * m + n * n;

          cout << a << " " << b << " " << c << "\n" ;
      }
  }

}
vector<vector<uint64_t>> gentriple (uint64_t limit) { // primitive triplet generation
  uint64_t n = 1, m = 2;
  uint64_t a, b, c;
  vector<vector<uint64_t>> tri;

  while ((m * m + 1) < limit) {
      // for (n = 1 + m % 2 ; n < m && n * n + m * m <= N ; n += 2) {}
      if (n >= m) n = m % 2, m = m + 1;

      c = m * m + n * n;

      if (c >= limit) {
          n = m;
      } else {
          if (__gcd(m,n) == 1) {
            // a = m * m - n * n;
            // b = 2 * m * n;
            // cout << a << " " << b << " " << c << "\n" ;
          }
      }
      n += 2;
  }

  return tri;
}

void pythtriple (uint64_t fs, uint64_t nd, uint64_t limit, atomic<uint64_t> &cnt) {
    uint64_t m, n;
    uint64_t c;

    for (m = fs; m < nd; m++) {
        for (n = 1 + m& 1; n < m ; n += 2) {
            if (__gcd (m,n) != 1) continue;
            c = m * m + n * n;
            if (c > limit) break;

            cnt++;
        }
    }
}

uint64_t count_triples (uint64_t limit) {
  const uint64_t nthread = thread::hardware_concurrency();
  uint64_t mlim = sqrt(limit) + 1;
  thread pool[nthread];
  atomic<uint64_t> cnt = 0;

  for (int i = 0; i < nthread; i++) {
      uint64_t fs = (mlim / nthread) * i, nd = (mlim / nthread) * (i + 1);
      pool[i] = thread (pythtriple, fs, nd, limit, std::ref (cnt));
  }

  for (thread &th : pool) {
      if (th.joinable()) {
          th.join();
      }
  }

  return cnt;
}

int main () {

    chrono::steady_clock::time_point alpha = chrono::steady_clock::now (), end;
  /*

    pythagorean triplet with m > n, a = m * m - n * n, b = 2 * n * m, c = m * m + n * n;

    Problem 540 : Counting Primitive Pythagorean Triples
    Pythagorean triple  consists of three positive integers a, b and c satisfying a^2 + b^2 = c^2.
    The triple is called primitive if a, b and c are relatively prime.
    Let P(n) be the number of primitive Pythagorean triples with a <= b <= c <= n.

    For example P(20) = 3, since there are three triples: (3,4,5), (5,12,13) and (8,15,17).

    You are given that P(10^6) = 159139.
    Find P(3141592653589793)

    1^1 => 1
    1^2 => 16
    1^3 => 158
    1^4 => 1593
    1^5 => 15919
    1^6 => 159139
    1^7 => 1591579
    1^8 => 15915492
    1^9 => 159154994

    x <<= 1; // Multiply x by 2
    x >>= 1; // Divide x by 2
    a ^= b ^= a ^= b // swap a and b

    */
    // vector<int> fun = {1,16,158,1593,15919,159139,1591579,15915492};

    int64_t limit = 100000000;
    limit = 100;

    int mlim = sqrt(limit) + 1;

    std::vector<int64_t> phi (mlim + 1);

    for (int64_t i = 0; i <= mlim; i++) {
        phi[i] = i;
    }

    for (int64_t i = 2; i <= mlim; i++) {
        if (phi[i] == i) {
            for (int64_t j = i; j <= mlim; j += i)
                phi[j] -= phi[j] / i;
        }
    }

    int64_t cnt = 0;
    int64_t c;

    for (int64_t m = 2; m < mlim; m++) {
        for (int64_t n = 1 + m& 1; n < m ; n += 2) {
            // if (phi[n] == phi[m]) {
                if (__gcd (m,n) != 1) {
                  cout << m << " " << n << endl;
                    continue;
                } else {
                }

                c = m * m + n * n;
                if (c > limit) break;

                cnt++;
        }
    }


    // for (int64_t i = 0; i <= mlim; i++) {
    //     cout << phi[i]<< " ";
    // }


    cout << "count => " << cnt;

    end = chrono::steady_clock::now ();
    std::chrono::duration<double> elapsed = end - alpha;
    cout << "\nDuration " << fixed << elapsed.count() << " ms" << endl;
}
