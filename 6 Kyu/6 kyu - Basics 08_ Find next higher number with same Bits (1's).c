#include <stdlib.h>

int next_higher(int value) {
    int i = value, ref = __builtin_popcount (value);

    while (i++)
        if (__builtin_popcount (i) == ref)
            return i;

    return 0;
}
