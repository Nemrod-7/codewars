#include <cstdint>

uint64_t pisano_period (uint64_t n);
////////////////////////////////////////////////////////////////////////////////
namespace Assert {
    void That (uint64_t actual, uint64_t expect) {

        if (actual != expect) {
            std::cout << " actual : " << actual;
            std::cout << " expected : " << expect;
            std::cout << "\n";
        }
    }
}
uint64_t Equals (uint64_t x ) { return x; }

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

}
////////////////////////////////////////////////////////////////////////////////

class show {
    public:
        static void factors (const std::vector<std::pair<uint64_t,uint64_t>> &fac) {
            for (auto &[p,e] : fac) {
                if (e == 1) {
                    std::cout << p << " ";
                } else {
                    std::cout << p << "^" << e << " ";
                }
            }
        }
        static void vect (const std::vector<uint64_t> &vc) {
            for (auto &p : vc) {
                std::cout << p << " ";
            }
        }
};


template <class T> std::ostream &operator << (std::ostream &stream, const std::vector<T> &obj) {
    for (auto &it : obj) {
        stream << it << " ";
    }
    return stream;
}
template<class T> std::ostream &operator << (std::ostream &stream, const std::pair<T,T> &obj) {
    stream << "[" << obj.first << "," << obj.second << "]";
    return stream;
}
/*//////////////////////////////Arkive////////////////////////////////////
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
*/
