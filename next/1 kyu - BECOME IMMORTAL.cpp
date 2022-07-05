#include <iostream>
#include <iomanip>

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
int64_t sumr (int64_t m, int64_t l) {
    if (l >= m) return 0;
    return (m - l) * 0.5 * (m - l - 1);
}
int64_t highpow (int64_t x) {
    int64_t t = 1;
    while (t < x) t <<= 1;
    return t;
}
int64_t highpow2 (int64_t x) {
    return pow (2, (int64_t) log2 (x) );
}

int64_t calc (int64_t m0, int64_t n0, int64_t m, int64_t n, int64_t l) {
    if (m == m0) return 0;
    int64_t cnt = 0, lm = highpow2 (m - m0);
    cnt = sumr (lm + (m0^n0), l);
    //cout << cnt << " " << (m0^n0) << endl;
    cout << m0 << " " << n0 << " " << m << " " << n << endl;

    if (l < (m0^n0)) {
        cnt  -= sumr ((m0^n0), l);
    }

    if ((n - n0) > lm) {
        cnt *= lm;
        cnt += calc (lm + m0, n0, m, lm + n0, l);
        cnt += calc (m0, lm + n0, lm + m0, n, l);
        cnt += calc (lm + m0, lm + n0, m, n, l);

    } else {
        int64_t len = abs (n - n0);

        if (len)
            cnt *= len;
      //  cnt = (sumr (lm + (m0^n0), l)) * (n - n0);
    }
    return cnt;
}

int64_t elder_age2 (int64_t m, int64_t n, int64_t l, int64_t t) {

    if (m == 0 || n == 0) return 0;
    int64_t cnt = 0;

    cnt = calc (0,0,m,n,l);
    /*
    for (int64_t y = 0; y < n; y++) {
        for (int64_t x = 0; x < m; x++) {
            int64_t num =  max (static_cast<int64_t> (0), (x ^ y) - l);
            cout << setw (2) <<  num << " ";
            cnt += num;
        }
        cout << endl;
    }

    /*
    */

    return cnt % t;
}

int64_t sumr2 (int64_t a, int64_t b) {
    return (a > b) ? 0 : ((a + b) * 0.5 * (b - a + 1));
}
int64_t mxpower (int64_t x) {
    int64_t k = 1;
    while (k < x) k <<= 1;
    return k;
}

int64_t elder_age (int64_t m, int64_t n, int64_t l, int64_t t) {

    if (m > n) swap (m,n);
    int64_t mp = mxpower (m), np = mxpower (n), cnt;
    if (m == 0 or l > np) return 0;
    // cout << m << " " << n << ' ' << npdl;

    if (mp == np) {
        //  cout << sumr2 (0, mp - l - 1) * (m + n - mp) + elder_age (mp - m, np - n, l, t) % t << npdl;
        return (sumr2 (0, mp - l - 1) * (m + n - mp) + elder_age (mp - m, np - n, l, t)) % t;

    } else {
        mp = (np / 2);
        int64_t a = (sumr2  (0, np - l - 1) * m);
        int64_t b = ((np - n) * sumr2  (max (mp - l, static_cast<int64_t> (0)), np - l - 1));
        //cout << sumr2  (0, np - l - 1) * m << npdl;
        cnt = (a - b);
        //cout << a << "\n" << b << "\n\n";
        //cout << cnt << npdl;
        if (l <= mp) {
            //cout << elder_age (mp - m, np - n, 0, t) + (mp - l) * (mp - m) * (np - n) << npdl;
            cnt += (elder_age (mp - m, np - n, 0, t) + (mp - l) * (mp - m) * (np - n));
            //cout << (mp - l) * (mp - m) * (np - n) << ' ';
        } else {
            //cout <<  elder_age (mp - m, np - n, l - mp, t) << ' ';
            cnt += (elder_age (mp - m, np - n, l - mp, t)) ;
        }

    }

    return cnt % t;
}

int main () {

  auto start = std::chrono::high_resolution_clock::now();
  //elder_age (28827050410, 35165045587, 7109602, 13719506);

  int64_t m = 7, n = 7, l = 0;
  int64_t m0 = 0, n0 = 0;
  int64_t lm = highpow2 (m - m0), cnt = 0;

    Assert::That(elder_age (8, 5, 1, 100), Equals(5));
    Assert::That(elder_age (8,8,0,100007), Equals(224));
    Assert::That(elder_age (25,31,0,100007), Equals(11925));
    Assert::That(elder_age (5,45,3,1000007), Equals(4323));
    Assert::That(elder_age(31,39,7,2345), Equals(1586));
    Assert::That(elder_age (545,435,342,1000007), Equals(808451));
    /*
    */

    Assert::That(elder_age (28827050410, 35165045587, 7109602, 13719506), Equals(5456283));

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "\nProcess took " << elapsed.count()  << " ms" << std::endl;
}
