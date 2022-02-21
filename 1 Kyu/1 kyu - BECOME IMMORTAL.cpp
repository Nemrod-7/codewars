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

int64_t mult (int64_t x, int64_t y, int64_t z, int64_t m) { return ((((x % m) * (y % m)) % m) * (z % m) % m); }
int64_t highpow (int64_t x) { return pow (2, static_cast<int64_t> (log2 (x))); }
int64_t elder_age (int64_t m, int64_t n, int64_t l, int64_t t) {
    if (m == 0 or n == 0) return 0;
    if (m < n) std::swap (m,n);

    const int64_t pm = highpow (m), pn = highpow (n);
    const int64_t nil = 0, opml = std::max (nil, pm - l) % t, olpm = std::max (nil, l - pm) % t, opml1 = std::max (nil, pm - l - 1) % t;

    int64_t fst = static_cast<int64_t>(opml * 0.5 * opml1) % t;
    int64_t scd = mult (std::min (pm, n), opml, m - pm, t);
    cout << m <<  " " << n << endl;
    //cout << opml << ' ' << olpm << ' ' << opml1  <<endl;
    if (pm < n) {
        fst *= pm;
        fst += mult (pm , std::max (int64_t (0), n - pm) , opml, t);

        scd += elder_age (n - pm, pm, olpm , t);
        scd += elder_age (m - pm, pn, olpm , t);
        scd += elder_age (m - pm, n - pn, l, t);

    } else {
        fst *= n;

        scd += elder_age (m - pm, n, olpm , t);
      /*
        */
    }

    return (fst + scd) % t;
}

int main () {

  auto start = std::chrono::high_resolution_clock::now();
  //elder_age (28827050410, 35165045587, 7109602, 13719506);

    elder_age (7,7,0,100);
  /*
    Assert::That(elder_age (8, 5, 1, 100), Equals(5));
    Assert::That(elder_age (8,8,0,100007), Equals(224));
    Assert::That(elder_age (25,31,0,100007), Equals(11925));
    Assert::That(elder_age (5,45,3,1000007), Equals(4323));
    Assert::That(elder_age (31,39,7,2345), Equals(1586));
    Assert::That(elder_age (545,435,342,1000007), Equals(808451));

    Assert::That(elder_age (28827050410, 35165045587, 7109602, 13719506), Equals(5456283));
    */

    //cout << elder_age (182, 503, 11, 76); // 19



  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "\nProcess took " << elapsed.count()  << " ms" << std::endl;
}
/*
int64_t calc (int64_t m0, int64_t n0, int64_t m, int64_t n, int64_t l) {
    if (m == m0) return 0;
    int64_t cnt = 0, lm = highpow2 (m - m0);
    cnt = sumr (lm + (m0^n0), l);
    //cout << cnt << " " << (m0^n0) << endl;
    //cout << m0 << " " << n0 << " " << m << " " << n << endl;

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

    //if (m == 0 || n == 0) return 0;
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


    return cnt % t;
}
*/
int64_t sumr (int64_t m, int64_t l, int64_t t) {
    int64_t n = ((m + l) % t) * 0.5 * ((l - m + 1) % t);
    return n % t;
}
int64_t highpow2 (int64_t x) {
    int64_t t = 1;
    while (t < x) t <<= 1;
    return t;
}
int64_t elder_age4 (int64_t m, int64_t n, int64_t l, int64_t t) {
    if (m == 0 or n == 0) return 0;
    if (m > n) std::swap (m,n);
    int64_t lm = highpow2 (m), ln = highpow2 (n), cnt;
    if (l > ln) return 0;

    if (lm == ln) {
        cnt = sumr (1, ln - l - 1, t);
        cnt *= (m + n - ln) % t;
        cnt += elder_age4 (ln - n, lm - m, l, t);
        //cout << sumr (1, ln - l - 1, t) * (m + n - ln) << endl;
    }

    if (lm < ln) {
        lm = floor (ln / 2);

        int64_t a = sumr (1, ln - l - 1, t) * (m % t);
        int64_t b = ((ln - n) % t);
        b *= sumr (std::max ( (int64_t) 0, lm - l), ln - l - 1, t);
        cnt = a - b;
        if (l <= lm) {
            cout << a << "\n" << b << "\n\n";

            cnt += mult (lm - l, lm - m, ln - n, t);
            cnt += elder_age4 (lm - m, ln - n, 0, t) ;
        } else {
            cnt += elder_age4 (lm - m, ln - n, l - lm, t);
        }
    }

    return cnt % t;
  }
