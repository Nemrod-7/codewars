#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>

#include <algorithm>
#include <chrono>

using namespace std;
// int gcd (int a, int b) { return b == 0 ? a : gcd (b, a % b); }
uint64_t gcd (uint64_t a, uint64_t b) {
    while(b) b ^= a ^= b ^= a %= b;
    return a;
}

void problem94 () {
  uint64_t limit = 1000000000;
  uint64_t side[2];

  uint64_t mlim = limit / 3 + 2;
  uint64_t sum = 0;
  uint64_t a, b = 0, c = 0;

  for (uint64_t m = 2; c < mlim; m++) {
      for (uint64_t n = 1; n < m ; n++) {
          side[0] = 2 * (m*m - n*n);
          side[1] = 4 * m * n;
          c = m * m + n * n;

          for (int k = 0; k < 2; k++) {
            if (c == side[k] - 1 || c == side[k] + 1) {
              unsigned b = side[k];
              uint64_t area = b / 2.0 * sqrt (c * c - (b * b) / 4.0);

              sum += (2 * b + c);
              cout << m << " " << n;
              // cout << b << " " << c << " :: " << c * c ;
              cout << endl;
            }
          }
      }
  }

  // 518408346
  cout << sum;
}
void trilet (uint64_t limit) {
  uint64_t a, b, c = 0;

  for (uint64_t m = 2; c < limit; m++) {
      for (uint64_t n = 1; n < m ; n++) {
          if ((m + n) % 2 == 0 || gcd(m,n) != 1) continue;

          a = m * m - n * n;
          b = 2 * m * n;
          c = m * m + n * n;

          cout << a << " " << b << " " << c << "\n" ;
      }
  }

}
vector<vector<uint64_t>> gentriple (uint64_t limit) { // primitive triplet generation
  uint64_t n = 1, m = 2;
  uint64_t a, b, c = 1;
  vector<vector<uint64_t>> tri;

  while ((m * m + 1) < limit) {
      if (n >= m) n = m % 2, m = m + 1;
      c = m * m + n * n;

      if (c >= limit) {
          n = m;
          continue;
      }

      if (gcd(m,n) == 1) {
          a = m * m - n * n;
          b = 2 * m * n;

          if (a > b) swap(a,b);

          tri.push_back({a,b,c});
          // cout << a << " " << b << " " << c << "\n" ;
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

    // pythagorean triplet with j > i
    // int a = m*m - n*n, b = 2 * n * m, c = m*m + n*n;

  */
    uint64_t limit = 100000000;

    uint64_t per = 0, cnt = 0;
    uint64_t n = 1, m = 2;
    uint64_t a, b, c = 1;

    for (m = 2; per < limit; m++) {
        for (n = 1; n < m ; n++) {
            if ((m + n) % 2 == 0 || gcd(m,n) != 1) continue;

            a = m * m - n * n;
            b = 2 * m * n;
            c = m * m + n * n;

            per = a + b + c;
            if (a == b) continue;
            if (a > b) swap (a,b);


            if (c % (b - a) == 0) {
                  // cout << cnt << " " << flush;
                cnt++;

                cout << a << " " << b << " " << c << "\n" ;
            }

            // cout << a << " " << b << " " << c << "\n" ;
        }
    }

    cout << "\ncount => " << cnt;

    end = chrono::steady_clock::now ();
    std::chrono::duration<double> elapsed = end - alpha;
    cout << "\nDuration " << fixed << elapsed.count() << " ms" << endl;
}
