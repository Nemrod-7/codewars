#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cstdint>
// #include "/home/wintermute/code/templates/Assert.hpp"
// 2 kyu - Pisano Period - Performance Edition
using namespace std;

using i64 = long int;
using u64 = uint64_t;
////////////////////////////////////////////////////////////////////////////////
namespace Assert {
    void That (u64 actual, u64 expect) {

        if (actual != expect) {
            std::cout << " actual : " << actual;
            std::cout << " expected : " << expect;
            std::cout << "\n";
        }
    }
}
u64 Equals (u64 x ) { return x; }
void Test() ;
////////////////////////////////////////////////////////////////////////////////

class show {
    public:
        static void factors(const vector<pair<u64,u64>> &fac) {
            for (auto &[p,e] : fac) {
                if (e == 1) {
                    cout << p << " ";
                } else {
                    cout << p << "^" << e << " ";
                }
            }
        }
};

template<class T> std::ostream &operator << (std::ostream &stream, const std::vector<T> &arr) {
    for (auto &it : arr) {
        stream << it << " ";
    }

    return stream;
}
////////////////////////////////////////////////////////////////////////////////
u64 mulmod (u64 a, u64 b, u64 mod) {
    u64 res = 0;

    while (b > 0) {
        if (b & 1) {
            res = (res + a) % mod;
        }
        a = (a * 2) % mod;
        b >>= 1;
    }

    return res;
}
u64 powmod (u64 a, u64 b, u64 mod) {
    u64 res = 1;
    a %= mod;

    while (b > 0) {
        if (b & 1) {
            res = mulmod(res, a, mod);
        }
        a = mulmod(a, a, mod);
        b >>= 1;
    }

    return res;
}

u64 gcd (u64 a, u64 b) {
    while (b) b ^= a ^= b ^= a %= b;
    return a;
}
u64 lcm (u64 a, u64 b) {
    return (a / gcd (a,b)) * b;
}

vector<pair<u64,u64>> factors (u64 n) {
    vector<pair<u64,u64>> vs;
    vector<int> pr = {2,3,5,7};
    const int wheel[48] = {2,4,2,4,6,2,6,4,2,4,6,6,2,6,4,2,6,4,6,8,4,2,4,2,4,8,6,4,6,2,4,6,2,6,6,4,2,4,6,2,6,4,2,4,2,10,2,10};

    for (auto &i : pr) {
        if (n % i == 0) {
            int ex = 0;
            while (n % i == 0) {
                n /= i, ex++;
            }
            vs.push_back({i,ex});
        }
    }

    for (u64 i = 11, t = 0; i * i <= n; i += wheel[t], t = t == 47 ? 0 : t + 1) {
        if (n % i == 0) {
            int ex = 0;
            while (n % i == 0) {
                n /= i, ex++;
            }
            vs.push_back({i,ex});
        }
    }

    if (n > 1) vs.push_back({n,1});
    return vs;
}
vector<u64> divisors (u64 n) {
    vector<u64> divisor = {1};
    vector<pair<u64,u64>> fac = factors(n);

    for (auto &[pr,ex] : fac) {
        int mult = 1, end = divisor.size();

        for (int j = 0; j < ex; j++) {
            mult *= pr;

            for (int k = 0; k < end; k++) {
                divisor.push_back(divisor[k] * mult);
            }
        }
    }

    sort(divisor.begin(), divisor.end());
    return divisor;
}

pair<int64_t,int64_t> fibonacci (int64_t n, int64_t mod) { // Function calculate the N-th fibonacci number using fast doubling method

    if (n == 0) return {0,1};
    auto [f0, f1] = fibonacci ((n / 2), mod);
    int64_t c = 2 * f1 - f0;

    if (c < 0) c += mod;
    c = (f0 * c) % mod; // As F(2n) = F(n)[2F(n+1) – F(n)]
    int64_t d = (f0 * f0 + f1 * f1) % mod; // As F(2n + 1) = F(n)^2 + F(n+1)^2

    return (n % 2 == 0) ? pair<int64_t,int64_t> {c, d} : pair<int64_t,int64_t> {d, c + d};
}

u64 pisano_prime2 (u64 m) {

    const pair<int64_t, int64_t> period = {0,1};
    u64 cycle = 2 * m + 2;
    vector<u64> divisor, d1 = divisors(m - 1);

    if ((m - 1) % 10 == 0 || (m + 1) % 10 == 0) {
        for (auto &d : d1) {
            if (d % 2 == 0) {
                divisor.push_back(d);
            }
        }

    } else {
        vector<u64> d2 = divisors(2 * m + 2);

        for (auto &d : d2) {
            if (find(d1.begin(), d1.end(), d) == d1.end()) {
                divisor.push_back(d);
            }
        }
    }

    for (auto &d : divisor) {
        if (fibonacci(d, m) == period) {
            cycle = min(d,cycle);
        }
    }

    return cycle;
}

u64 pisano_period (u64 n) { // OEIS A001175 pisano period of fib(i) mod n
    if (n == 1) return 1;
    if (n == 2) return 3;
    if (n == 5) return 20;

    u64 cycle = 1;
    vector<pair<u64,u64>> fac = factors(n);

    if (fac.size() == 1 && fac[0].second == 1) { // if n is a prime number
        cycle = pisano_prime2(n);

    } else {
        // if p is a prime number and k is interger power : pisano(p^k) == p^(k-1) * pisano (p)
        for (auto &[p,ex] : fac) {
            int an = pisano_period(p) * powmod(p, ex - 1, n);

            cycle = lcm(cycle,an) ;
        }
    }

    return cycle;
}
int main () {

    const vector<int> pi = {1, 1, 3, 8, 6, 20, 24, 16, 12, 24, 60, 10, 24, 28, 48, 40, 24, 36, 24, 18, 60, 16, 30, 48, 24, 100, 84, 72, 48, 14, 120, 30, 48, 40, 36, 80, 24, 76, 18, 56, 60, 40, 48, 88, 30, 120, 48, 32, 24, 112, 300, 72, 84, 108, 72, 20, 48, 72, 42, 58, 120, 60, 30, 48, 96, 140, 120, 136};

    u64 ref = 2438389198053, res = 10624179384;
    u64 num;

    for (u64 i = 2; i < pi.size(); i++) {
        int cycle = pisano_period(i);

        if (cycle != pi[i]) {
            cout << i << " : " << cycle << " " << pi[i] << "\n";
        }
    }

    cout << "\nexit\n";
    //Assert::That(pisano_period(2438389198053), Equals(10624179384));
}

void Test () {

    {
        Assert::That(pisano_period(5), Equals(20));
        Assert::That(pisano_period(47), Equals(32));
        Assert::That(pisano_period(20), Equals(60));
        Assert::That(pisano_period(101), Equals(50));
    }

    {
        Assert::That(pisano_period(4253), Equals(8508));
        Assert::That(pisano_period(19661), Equals(9830));
        Assert::That(pisano_period(14207), Equals(28416));
        Assert::That(pisano_period(761), Equals(380));
    }

    {
        Assert::That(pisano_period(440119), Equals(444496));
        Assert::That(pisano_period(1672377), Equals(160720));
        Assert::That(pisano_period(4020378), Equals(483720));
        Assert::That(pisano_period(14451817), Equals(1804224));
    }

    {
        Assert::That(pisano_period(1818176898), Equals(1408320));
        Assert::That(pisano_period(2438389198053), Equals(10624179384));
    }

    /*
    */
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
u64 pisano_prime (u64 n) {
    if (n == 2) return 3;
    if (n == 5) return 20;

    u64 a1 = 1, a0 = 1, tmp = 1;
    u64 cycle = 1;

    while (a0 != 0 || a1 != 1) {
        cycle++;
        tmp = (a0 + a1) % n, a0 = a1, a1 = tmp;
    }

    return cycle;
}
u64 ipow (u64 base, u64 exp) {
    u64 power = 1;

    while (exp) {
        if (exp &1) power *= base;

        exp >>= 1;
        base *= base;
    }

    return power;
}

void evaluate() {
  const vector end = {0,1};
  int a1 = 1, an = 1, a0 = 0;

  u64 ref = 2438389198053, res = 10624179384;
  u64 num;
  num = 3 * 3 * 3;
  num = 17;
  num = 5312394767;

  int n = 10000;
  vector<int> sieve (n);
  // ofstream ofs ("notes", ios::out);

  for (u64 i = 2; i < n; i++) {
      if (sieve[i] == 0) {

          u64 p1 = pisano_prime(i);
          u64 p2 = pisano_prime2(i);

          if (p1 != p2) {
              cout << setw(2) << i << " : ";
              cout << "[" << setw(3) << p1 << "]";
              cout << p2;
              cout << "\n";
          }

          for (u64 j = i * i; j < n; j += i) {
              sieve[j] = true;
          }
      }
  }
}
int pisano1 (int m) {

    if (m == 1) return 1;
    int cycle = 1;

    return cycle  ;
}
bool is_prime (i64 p) {
    if (p < 2) return false;
    if (0 == p % 2) return 2 == p;
    if (0 == p % 3) return 3 == p;

    i64 d = 5;

    while (d * d <= p) {
        if (0 == p % d) return false;
        d += 2;
        if (0 == p % d) return false;
        d += 4;
    }
    return true;
}
i64 period(const std::vector<int> &v) { // detect cycle in a generic sequence

    for (i64 i = 1; i < v.size(); i++) {
        i64 j = 0;

        while (j < v.size() - i && v[j] == v[j + i]) ++j;

        if (j + i == v.size()) {
            return i;
        }
    }

    return 0;
}
vector<int> fibonacci (int n, int m) {
    vector<int> seq (n);
    seq[0] = 0, seq[1] = 1;

    for (int i = 2; i < n; i++) {
        seq[i] = ( seq[i-2] + seq[i-1]) % m ;
    }

    return seq;
}
