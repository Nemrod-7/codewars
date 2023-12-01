#include <iostream>
#include <vector>
#include <cmath>

using namespace std;
using ull = unsigned long long;

namespace Assert {

    enum {unequal, equal};

    template<class T> 
        struct result {
            int type;
            T x;
            double delta;
        };

        static void overflow (double a, double b) {
            double limit = std::numeric_limits<double>::max() - a;
            if (a > limit) throw std::overflow_error ("integer overflow\n");
        }

    template<class T> 
        void That(const T &actual, const result<T> &expect, const std::string &msg = "") {
            bool result = actual == expect.x;
            
            if (result != expect.type) {
                if (msg == "") {
                    std::cout << "\nactual   : " << actual;
                    std::cout << "\nexpected : " << expect.x;
                }
            }
        }

        template<class T>  void That (const T& a, const T& b) {
            if (a != b) {
                std::cout << "actual : " << a << " expected : " << b;
                std::cout << std::endl;
            }
        }
        void That (const double &act, const result<double>  &exp) {
            static int cycle = 0;

            if (fabs(exp.x - act) > exp.delta) {
                printf ("cycle: %i \nactual   : %.012f \nexpected : %.012f\n", cycle, act, exp.x);
            }
            cycle++;
        }
        template<class T> void equals_eq (const T &res, const T &exp, std::string str) {

            static int cycle;
            cycle++;
            if (res != exp) {
                std::cout << "test " << cycle << " : " << str << " => ";
                std::cout << "got : " << res << " ";
                std::cout << "exp : " << exp << "\n";
            }
        }
        template<class T> static void That (const std::vector<T> &a, const result<T> &b) {
            bool equal = true;

            for (int i = 0; i < a.size(); i++) {
                if (fabs(a[i] - b.x[i]) > b.delta) {
                    equal = false;
                }
            }

            if (equal == false) {
                std::cout << "\n actual   : ";
                for (auto &it : a) std::cout << it << " ";

                std::cout << "\n expected : ";
                for (auto &it : b) std::cout << it << " ";

                std::cout << std::endl;
            }
        }

}

std::string ExtraMessage(const char *src) { return src; }
template<class T> Assert::result<T> Equals (const T &x) { return { Assert::equal, x }; }
template<class T> Assert::result<T> EqualsContainer (const T& x) { return { Assert::equal, x }; }
template<class T> Assert::result<T> IsNotEqualTo (const T &x) { return { Assert::unequal, x }; }
template<class T> Assert::result<T> EqualsWithDelta (const T &x, const double &delta) { return { Assert::equal, x, delta }; }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ull mulmod (ull a, ull b, ull mod) {
    ull res = 0;

    while (b > 0) {
        if (b &1) {
            res = (res + a) % mod;
        }
        a = (a * 2) % mod;
        b >>= 1;
    }

    return res;
}

ull powmod (ull a, ull b, ull mod) {
    ull res = 1;
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

int tower (ull base, ull height, ull mod) {
    if (mod == 1) return 0;
    if (base == 1 || height == 0) return 1;

    ull x = base;

    for (ull i = 0; i < height; i++) {

        x = powmod(x,base, mod);
    }

    return x;
}

void test () {

    Assert::That(tower(729, 0, 1), Equals(0), ExtraMessage(" Incorrect answer for base=729 exp=0 mod=1"));
    Assert::That(tower(729, 0, 2), Equals(1), ExtraMessage(" Incorrect answer for base=729 exp=0 mod=2"));
    Assert::That(tower(1, 897, 8934279), Equals(1), ExtraMessage(" Incorrect answer for base=1 exp=897 mod=8934279"));

    Assert::That(tower(3, 3, 25), Equals(12), ExtraMessage(" Incorrect answer for base=3 exp=3 mod=25"));
    Assert::That(tower(2, 2, 1000), Equals(4), ExtraMessage(" Incorrect answer for base=2 exp=2 mod=1000"));
    Assert::That(tower(2, 3, 100000), Equals(16), ExtraMessage(" Incorrect answer for base=2 exp=3 mod=100000"));
    Assert::That(tower(2, 4, 100000000), Equals(65536), ExtraMessage(" Incorrect answer for base=2 exp=4 mod=100000000"));
    Assert::That(tower(4, 2, 10000000), Equals(256), ExtraMessage(" Incorrect answer for base=4 exp=2 mod=10000000"));
    Assert::That(tower(4, 3, 10), Equals(6), ExtraMessage(" Incorrect answer for base=4 exp=3 mod=10"));
    Assert::That(tower(7, 1, 5), Equals(2), ExtraMessage(" Incorrect answer for base=7 exp=1 mod=5"));

    /*
       Assert::That(tower(28, 3, 25), Is().Not().EqualTo(tower(28 % 25, 3, 25)), ExtraMessage(" Incorrect answer for base=28 exp=3 mod=25"));
       Assert::That(tower(13, 3, 31), Is().Not().EqualTo(3), ExtraMessage(" Incorrect answer for base=13 exp=3 mod=31"));
       Assert::That(tower(13, 3, 31), Equals(11), ExtraMessage(" Incorrect answer for base=13 exp=3 mod=31"));
       Assert::That(tower(3, 4, 1001), Equals(482), ExtraMessage(" Incorrect answer for base=3 exp=4 mod=1001"));
       Assert::That(tower(2, 6, 1001), Equals(471), ExtraMessage(" Incorrect answer for base=2 exp=6 mod=1001"));
       */


}

int main () {



}
