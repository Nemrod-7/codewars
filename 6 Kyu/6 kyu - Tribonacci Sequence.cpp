#include <numeric>

std::vector<int> tribonacci(std::vector<int> sig, int n) {
    std::vector<int> v;

    for (size_t i = 0; i < n; ++i) {
        if (i < sig.size())
            v.push_back(sig[i]);
        else
            v.push_back (std::accumulate(&v[i - 3], &v[i], 0));
    }
    return v;
}
