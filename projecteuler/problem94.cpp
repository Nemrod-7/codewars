#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>

#include <algorithm>
#include <chrono>
#include<bits/stdc++.h>

using namespace std;
// int gcd (int a, int b) { return b == 0 ? a : gcd (b, a % b); }
uint64_t gcd (uint64_t a, uint64_t b) {
    while (b) b ^= a ^= b ^= a %= b;
    return a;
}

void trilet (uint64_t limit) {
  uint64_t a, b, c = 0;

  for (uint64_t m = 2; (m * m + 1) < limit; m++) {
      for (uint64_t n = 1; n < m ; n++) {
          if ((m + n) % 2 == 0 || gcd (m,n) != 1) continue; // primitive right triangle : cannot be scaled to another smaller right triangle

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
          if (gcd(m,n) == 1) {
            // a = m * m - n * n;
            // b = 2 * m * n;
            // cout << a << " " << b << " " << c << "\n" ;
          }
      }
      n += 2;
  }

  return tri;
}

int main () {

    chrono::steady_clock::time_point alpha = chrono::steady_clock::now (), end;

  /*
           ^
          /|\
         / | \
      c /  |h \ c
       /   |   \
      /____|____\
        b/2  b/2

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

    1 16 158 1593 15919 159139 1591579 15915492
    */

    vector<int> fun = {1,16,158,1593,15919,159139,1591579,15915492};
    int64_t limit = 100000000;
    limit = 1e8;
    // sqrt(limit - m * m);
    const double pi = 3.14159265358979323846;

    uint64_t n = 1, m = 2;
    uint64_t a, b, c;
    uint64_t cnt = 0;


    for (uint64_t m = 2; (m * m + 1) < limit; m++) {
        uint64_t nd = min((uint64_t) sqrt(limit - m * m) + 1, m);
        for (uint64_t n = 1 + m % 2; n < nd ; n += 2) {
            // if (gcd (m,n) != 1) continue;
            // c = m * m + n * n;
            // if (c >= limit) break;

            cnt++;
        }
    }

    // while ((m * m + 1) < limit) {
    //     if (n >= m) n = m % 2, m++;
    //     c = m * m + n * n;
    //
    //     if (c >= limit) {
    //         n = m;
    //     } else {
    //         if (gcd (m,n) == 1) {
    //             // a = m * m - n * n;
    //             // b = 2 * m * n;
    //             // cout << a << " " << b << " " << c << "\n" ;
    //             cnt++;
    //         }
    //         n += 2;
    //     }
    // }

    cout << "count => " << cnt;

    end = chrono::steady_clock::now ();
    std::chrono::duration<double> elapsed = end - alpha;
    cout << "\nDuration " << fixed << elapsed.count() << " ms" << endl;
}
