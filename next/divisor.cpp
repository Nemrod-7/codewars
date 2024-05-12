#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>

using namespace std;

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

int64_t mulmod (int64_t a, int64_t b, int64_t mod) {
    int64_t res = 0;

    while (b > 0) {
        if (b & 1) {
            res = (res + a) % mod;
        }
        a = (a * 2) % mod;
        b >>= 1;
    }

    return res;
}
int64_t powmod (int64_t a, int64_t b, int64_t mod) {
    int64_t res = 1;
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

pair<uint64_t,uint64_t> fibonacci (uint64_t n, uint64_t m) { // Function calculate the N-th fibonacci number using fast doubling method

    if (n == 0) return {0,1};

    auto [f0,f1] = fibonacci(n >> 1, m);
    int64_t c = 2 * f1 - f0;

    if (c < 0) c += m;

    c = (f0 * c) % m; // F(2n) = F(n)[2F(n+1) – F(n)]
    uint64_t d = (f0 * f0 + f1 * f1) % m; // F(2n + 1) = F(n)^2 + F(n+1)^2

    return (n % 2 == 0) ? pair<uint64_t,uint64_t> {c, d} : pair<uint64_t,uint64_t> {d, c + d};
}



int main () {

    uint64_t num = 181; // period = 32
    const pair<uint64_t, uint64_t> period = {0,1};
     std::vector<uint64_t> div = divisors (num - 1);

     for (auto n : div) {
         cout << fibonacci (n, num) ;

     }



    cout << "\nexit\n";
}
