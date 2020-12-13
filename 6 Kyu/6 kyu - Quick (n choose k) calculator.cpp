#include <iostream>

typedef long long unsigned ull;

ull choose (ull n, ull k) {
    if (k == 0 || k == n) return 1;
    if (k  < 0 || k  > n) return 0;

    long double mul = 1;
    if (k > (n / 2)) k = n - k;

    for (ull  i = 1; i <= k; ++i)
        mul = mul * (n + 1 - i) / i;

    return mul;
}
