#include <inttypes.h>
#include <math.h>

typedef uint64_t u64;

u64 modpow (u64 base, u64 exp, u64 mod) {

    u64 res = 1;

    while (exp) {
        if (exp &1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }

    return res;
}
