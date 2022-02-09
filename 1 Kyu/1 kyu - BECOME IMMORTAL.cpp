#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

using namespace std;
/////////////////////////////////Assert/////////////////////////////////////////
class Assert {
    public :
        static void That (const int64_t &a, const int64_t &b) {
            if (a != b) {
                cout << "actual : " << a << " expected : " << b;
                cout << endl;
            }
        }
};
int64_t Equals (const int64_t &entry) { return entry;}
void Test ();
////////////////////////////////////////////////////////////////////////////////
bool powerof2 (int64_t n) { return n && (!(n & (n-1))); }
int64_t highpow2 (int64_t n) { return powl (2, floor (log2 (n))); }

int64_t elder_age (int64_t m, int64_t n, int64_t l, int64_t t) {
    int64_t cnt = 0;

    for (int64_t y = 0; y < n; y++) {
        for (int64_t x = 0; x < m; x++) {
            int64_t num = (x ^ y) - l;

            if (num > 0) cnt += num;
        }

    }
    return cnt % t;
}
int64_t elder_age2 (int64_t m, int64_t n, int64_t l, int64_t t) {

    if (m < n) swap (m, n);
    int64_t lm = highpow2 (m), ln = highpow2 (n);

    int64_t total = 0, cnt2 = (ln * (ln - 1) / 2) * ln;

    for (int64_t y = 0; y < n; y++) {
        for (int64_t x = 0; x < m; x++) {

            int64_t num = (x ^ y);
            total += num;
            if (x == ln) cout << " ";

            //if (x >= cube || y >= cube) cnt2 += num;
            cout << num << " ";
        }
        if (y == ln - 1) cout << endl;
        cout << endl;
    }
    /*
    */
    cout << cnt2 << " " << total << " " << (m * (m - 1) / 2) * n;
    cout <<  "\n\n";

    return total;
}

uint64_t larger_pow (uint64_t x) {
    uint64_t t = 1;
    while (t < x) t <<= 1;
    return t;
}

uint64_t rsum (uint64_t l, uint64_t r) {
    int64_t a = l + r, b = r - l + 1;
    return floor (a * 0.5 * b);
}

int64_t elder_age3 (int64_t m, int64_t n, int64_t l, int64_t t) {

    if (m == 0 || n == 0) return 0;
    if (m > n) swap (m,n);

    uint64_t cnt = 0;
    uint64_t lm = larger_pow (m), ln = larger_pow (n);

    //if (l > ln) return 0;
    if (lm == ln)
        cnt = (rsum (1, ln - l - 1) * (m + n - ln) + elder_age3 (ln - n, lm - m, l, t));

    if (lm < ln) {
        //cout << ln << endl;
        lm = floor (ln / 2);
        cnt = rsum (1, ln - l - 1) * m - (ln - n) * rsum (max ((uint64_t) 0, lm - l), ln - l - 1);

        if (l <= lm) {
            cnt += (lm - l) * (lm - m) * (ln - n) + elder_age3 (lm - m, ln - n, 0, t);
        } else {
            cnt += elder_age3 (lm - m, ln - n, l - lm, t);
        }
    }

    //cout << cnt << endl;
    return cnt % t;
}

int main () {

  auto start = std::chrono::high_resolution_clock::now();
  //elder_age2(28827050410, 35165045587, 7109602, 13719506);

  Assert::That(elder_age3 (28827050410, 35165045587, 7109602, 13719506), Equals (5456283));

  Assert::That(elder_age3 (8, 5, 1, 100), Equals(5));
  Assert::That(elder_age3 (8,8,0,100007), Equals(224));
  Assert::That(elder_age3 (25,31,0,100007), Equals(11925));
  Assert::That(elder_age3 (5,45,3,1000007), Equals(4323));
  Assert::That(elder_age3 (31,39,7,2345), Equals(1586));
  Assert::That(elder_age3 (545,435,342,1000007), Equals(808451));
  /*
  */


  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "\nProcess took " << elapsed.count()  << " ms" << std::endl;
}
