#include <stddef.h>

int positive_sum(const int *values, size_t count) {

    const int * const end = values + count;
    int sum = 0;

    if (count = 0)
        return 0;
    else
        for (;values < end; ++values) {
            (*values > 0) ? sum += *values : 0;
        }
    return sum;
}
