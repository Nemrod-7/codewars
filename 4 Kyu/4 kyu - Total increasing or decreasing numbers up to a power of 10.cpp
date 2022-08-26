//

typedef unsigned long long ull;

ull binomial (ull n, ull k) {

    if (k == 0 || k == n) return 1;
    if (k  < 0 || k  > n) return 0;

    ull res = 1;
    k = std::min (k, n - k);
    for (ull i = 1; i <= k; i++)
        res = res * (n + 1 - i) / i;

    return res;
}

ull total_inc_dec (unsigned int n) {
    return binomial (n + 9, n) + binomial (n + 10, n) - 1 - n * 10;
}
