#include <stdlib.h>

long long *tribonacci(const long long sig[3], size_t n) {

    if (n == 0) return NULL;

    long long *v = malloc (n * sizeof (long long));

    for (size_t i = 0; i < n; ++i)
        v[i] = (i < 3) ? sig[i] : (v[i - 3] + v[i - 2] + v[i - 1]);

    return v;
}
