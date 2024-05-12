#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cstdint>
// #include "/home/wintermute/code/templates/Assert.hpp"

using namespace std;

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
void Test() ;
////////////////////////////////////////////////////////////////////////////////
using i64 = long int;

class show {
    public:
        static void factors(const vector<pair<uint64_t,uint64_t>> &fac) {
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
uint64_t mulmod (uint64_t a, uint64_t b, uint64_t mod) {
    uint64_t res = 0;

    while (b > 0) {
        if (b & 1) {
            res = (res + a) % mod;
        }
        a = (a * 2) % mod;
        b >>= 1;
    }

    return res;
}
uint64_t powmod (uint64_t a, uint64_t b, uint64_t mod) {
    uint64_t res = 1;
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

int gcd (int a, int b) {
    while (b) b ^= a ^= b ^= a %= b;
    return a;
}
int lcm (int a, int b) {
    return (a / gcd (a,b)) * b;
}

int ipow (int base, int exp) {
    int power = 1;

    while (exp) {
        if (exp &1) power *= base;

        exp >>= 1;
        base *= base;
    }

    return power;
}
vector<pair<uint64_t,uint64_t>> factors (uint64_t n) {
    vector<pair<uint64_t,uint64_t>> vs;
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

    for (int i = 11, t = 2; i * i <= n; i += wheel[t], t = t == 47 ? 0 : t + 1) {
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
vector<uint64_t> divisors (uint64_t n) {
    vector<uint64_t> divisor = {1};
    vector<pair<uint64_t,uint64_t>> fac = factors(n);

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

pair<uint64_t, uint64_t> fibonacci (uint64_t n, uint64_t m) {
    if (n == 0) return {0, 1};

    pair<uint64_t, uint64_t> p = fibonacci (n >> 1, m);
    uint64_t c = ( p.first * (2 * p.second - p.first)) % m;
    uint64_t d = ( p.first * p.first + p.second * p.second) % m;

    return (n &1) ? pair { d , c + d } : pair { c, d };
}

uint64_t pisano_prime (uint64_t n) {
    if (n == 2) return 3;
    if (n == 5) return 20;
    
    uint64_t a1 = 1, a0 = 1, tmp = 1;
    uint64_t cycle = 1;

    while (a0 != 0 || a1 != 1) {
        cycle++;
        tmp = (a0 + a1) % n, a0 = a1, a1 = tmp;
    }

    return cycle;
}
uint64_t pisano_prime2 (uint64_t m) {

    const pair<uint64_t, uint64_t> period = {0,1};
    uint64_t cycle = 2 * m + 2;
    vector<uint64_t> divisor, d1 = divisors(m - 1);

    if ((m - 1) % 10 == 0 || (m + 1) % 10 == 0) {

        for (int i = 0; i < d1.size(); i++) {
            //if (d1[i] % 2 == 0) {
                divisor.push_back(d1[i]);
            //}
        }

    } else {
        vector<uint64_t> d2 = divisors(2 * m + 2);

        for (int i = 0; i < d2.size(); i++) {
            bool valid = true;

            for (int j = 0; j < d1.size(); j++) {
                if (d2[i] == d1[j]) {
                    valid = false;
                    break;
                }
            }

            if (valid) divisor.push_back(d2[i]);
        }
    }

    for (auto &d : divisor) {
        if (fibonacci(d, m) == period) {
            cycle = min(d,cycle);
        }
    }

    return cycle;
}

uint64_t pisano_period (uint64_t n) { // OEIS A001175 pisano period of fib(i) mod n
    if (n == 1) return 1;
    if (n == 2) return 3;
    if (n == 5) return 20;

    uint64_t cycle = 1;
    vector<pair<uint64_t,uint64_t>> fac = factors(n);

    show::factors(fac);

    if (fac.size() > 1) {
        // if p is a prime number and k is interger power : pisano(p^k) == p^(k-1) * pisano (p)
        for (auto &[p,ex] : fac) {
            int an = ipow(p, ex - 1) * pisano_period(p);
            cycle = lcm(cycle,an) ;
        }

    } else {
        cycle = pisano_prime(n);
    }


    return cycle;
}

int main () {

    const vector<int> pi = {1, 1, 3, 8, 6, 20, 24, 16, 12, 24, 60, 10, 24, 28, 48, 40, 24, 36, 24, 18, 60, 16, 30, 48, 24, 100, 84, 72, 48, 14, 120, 30, 48, 40, 36, 80, 24, 76, 18, 56, 60, 40, 48, 88, 30, 120, 48, 32, 24, 112, 300, 72, 84, 108, 72, 20, 48, 72, 42, 58, 120, 60, 30, 48, 96, 140, 120, 136};

    const vector end = {0,1};
    int a1 = 1, an = 1, a0 = 0;

    uint64_t ref = 2438389198053, res = 10624179384;
    uint64_t num;
    num = 3 * 3 * 3;
    num = 17;
    num = 5312394767;

    int n = 1000;
    vector<int> sieve (n);
    // ofstream ofs ("notes", ios::out);

    for (int i = 2; i < n; i++) {
        if (sieve[i] == 0) {

            int p1 = pisano_prime(i);
            int p2 = pisano_prime2(i);


            if (p1 != p2) {
                cout << setw(2) << i << " : ";
                cout << "[" << setw(3) << p1 << "]";
                cout << p2;
                cout << "\n";
            }


            for (int j = i * i; j < n; j += i) {
                sieve[j] = true;
            }
        }
    }

    // ofs.close();
    //evaluate();

    //pisano_prime(num);
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
void evaluate() {

    int n = 100;
    vector<int> sieve (n);

    for (int i = 2; i < n; i++) {
        if (sieve[i] == 0) {
            int cycle = pisano_prime(i);

            vector<int> bound = {i - 1, 2 * i + 2};
            cout << setw(2) << i << " : ";

            if (cycle == bound[0]) {
                cout << "[" << setw(3) << bound[0] << "][   ] ";
            } else if (cycle == bound[1]) {
                cout << "[   ][" << setw(3) << cycle << "] ";
            } else {
                cout << "[" <<setw(8) << cycle <<"] ";
            }

            if ((i - 1) % 5 == 0 || (i + 1) % 5 == 0) {
                cout << i - 1 << " ";
            }
            if ((i - 3) % 10 == 0 || (i + 3) % 10 == 0) {
                cout << 2 * i + 2 << "<";
            }

            cout << endl;


            for (int j = i * i; j < n; j += i) {
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
