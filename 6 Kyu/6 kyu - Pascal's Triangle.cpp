#include <vector>
#include <cmath>

long long binom (int n, int k) {
    if (k == 0 || k == n) return 1;
    if (k  < 0 || k  > n) return 0;

    long double res = 1;
    k = std::min (k, n - k);

    for (double i = 1; i <= k; i++)
        res = res * (n + 1 - i) / i;

    return static_cast<long long>(res);
}
std::vector<long long> pascalsTriangle (int size) {
    std::vector<long long> tri;

    for(int n = 0; n < size; ++n)
        for(int k = 0; k <= n; ++k)
            tri.push_back ((binom (n, k)));

    return tri;
}
