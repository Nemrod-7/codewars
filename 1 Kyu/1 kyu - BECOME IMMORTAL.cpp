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

int64_t highpow3 (int64_t x) {
    int64_t n = 1;
    while (x >= 2) { n *= 2, x /= 2; }
    return n;
}
uint64_t sumr2 (uint64_t m, uint64_t n, uint64_t l, uint64_t t) {

    uint64_t m2 = std::max (int64_t (0), int64_t (m - l)), n2 = std::max (int64_t (0),  int64_t (n - l));
    uint64_t fst = m2 + n2 - 1, scd = n2 - m2;

    fst % 2 ? scd /= 2 : fst /= 2;

    return ((fst % t) * (scd % t)) % t;
}
int64_t elder_age (int64_t m, int64_t n, int64_t l, int64_t t) {

    if (m == 0 or n == 0) return 0;
    if (m < n) std::swap (m,n);
    if (n == 1) return sumr2 (0, m, l, t);

    const int64_t pm = highpow3 (m), pn = highpow3 (n);
    const int64_t pml = std::max (int64_t (0), pm - l) % t;
  
    uint64_t cnt = sumr2 (0, pm, l, t);

    if (pm == pn) {       
      cnt *= (pm % t);

      uint64_t a = m + n, b = pm * 2;
      uint64_t c = sumr2 (pm , b, l, t);
      
      cnt += (c * ((a - b) % t)) % t;
      cnt += elder_age (m - pm, n - pn, l, t);

    } else {
      cnt *= (n % t);
      
      uint64_t a = ((pml % t) * ((m - pm) % t)) % t, b = n % t;
      
      cnt += (a * b) % t;
      cnt += elder_age (m - pm, n, std::max (int64_t (0), l - pm), t);
    }

    return cnt % t;
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
