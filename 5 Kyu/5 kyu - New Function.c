#include <stdlib.h>
#include <stdbool.h>
#define SIG(N,k) (N / k * (N / k + 1) * 0.5)

typedef unsigned long long int ull;

ull new_function(const ull num) {
    ull k = sqrt (num) + 1, csod = 0, d;

    while (k-->2) {
        d = num / k;
        csod += (d - k) * k + (d + k) * (d - k + 1) * 0.5;
    }

    return csod;
}
