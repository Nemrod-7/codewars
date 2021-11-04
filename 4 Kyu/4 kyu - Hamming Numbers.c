#include <stdint.h>

#define MIN(a,b) (((a) < (b)) ? (a) : (b))

uint64_t hamber(int n) {
    if (n < 7) return n;
    uint64_t seq[n], x2 = 2, x3 = 3, x5 = 5;;
    int i = 0, j = 0, k = 0;
    seq[i] = 1;

    for (int it = 0; it < n; it++) {
        seq[it] = MIN (x2, MIN(x3, x5));

        if (seq[it] == x2) x2 = seq[i++] * 2;
        if (seq[it] == x3) x3 = seq[j++] * 3;
        if (seq[it] == x5) x5 = seq[k++] * 5;
    }

    return seq[n - 2];
}
