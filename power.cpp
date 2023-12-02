#include <iostream>
#include <vector>
#include <cmath>

#include "/home/wintermute/code/templates/Assert.hpp"

using namespace std;
using ull = unsigned long long;

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

    ull res = base;

    for (ull i = 0; i < height; i++) {
        // res = (int)(pow(res,base)) % mod;
        cout << res << ' ';
        res = powmod(base, res, mod);
    }
    cout << "\n";
    return res;
}

int main () {
    //test();
    ull base = 2, height = 6, mod = 10000;
    ull x = base;

      cout << powmod(2,65536,1001);
      // tower (2, 65536,1001);
    // Assert::That(tower(2, 2, 1000), Equals(4), ExtraMessage(" Incorrect answer for base=2 exp=2 mod=1000"));
    // Assert::That(tower(2, 3, 100000), Equals(16), ExtraMessage(" Incorrect answer for base=2 exp=3 mod=100000"));
    // Assert::That(tower(2, 4, 100000000), Equals(65536), ExtraMessage(" Incorrect answer for base=2 exp=4 mod=100000000"));
    // Assert::That(tower(2, 6, 1001), Equals(471), ExtraMessage(" Incorrect answer for base=2 exp=6 mod=1001"));


}



void test () {

    Assert::That(tower(729, 0, 1), Equals(0), ExtraMessage(" Incorrect answer for base=729 exp=0 mod=1"));
    Assert::That(tower(729, 0, 2), Equals(1), ExtraMessage(" Incorrect answer for base=729 exp=0 mod=2"));
    Assert::That(tower(1, 897, 8934279), Equals(1), ExtraMessage(" Incorrect answer for base=1 exp=897 mod=8934279"));

    Assert::That(tower(2, 2, 1000), Equals(4), ExtraMessage(" Incorrect answer for base=2 exp=2 mod=1000"));
    Assert::That(tower(2, 3, 100000), Equals(16), ExtraMessage(" Incorrect answer for base=2 exp=3 mod=100000"));
    Assert::That(tower(2, 4, 100000000), Equals(65536), ExtraMessage(" Incorrect answer for base=2 exp=4 mod=100000000"));
    Assert::That(tower(2, 6, 1001), Equals(471), ExtraMessage(" Incorrect answer for base=2 exp=6 mod=1001"));
    Assert::That(tower(3, 3, 25), Equals(12), ExtraMessage(" Incorrect answer for base=3 exp=3 mod=25"));
    Assert::That(tower(4, 3, 10), Equals(6), ExtraMessage(" Incorrect answer for base=4 exp=3 mod=10"));
    Assert::That(tower(7, 1, 5), Equals(2), ExtraMessage(" Incorrect answer for base=7 exp=1 mod=5"));

    Assert::That(tower(13, 3, 31), Equals(11), ExtraMessage(" Incorrect answer for base=13 exp=3 mod=31"));
    Assert::That(tower(3, 4, 1001), Equals(482), ExtraMessage(" Incorrect answer for base=3 exp=4 mod=1001"));

    Assert::That(tower(13, 3, 31), IsNotEqualTo(3), ExtraMessage(" Incorrect answer for base=13 exp=3 mod=31"));
    Assert::That(tower(28, 3, 25), IsNotEqualTo(tower(28 % 25, 3, 25)), ExtraMessage(" Incorrect answer for base=28 exp=3 mod=25"));
    /*
    */

}
