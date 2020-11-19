#include <stdbool.h>

bool is_prime (long long num) {

    long long i;
    const long long end = sqrt (num);

    if (num <= 3) return num > 1;
    if (num % 2 == 0 || num % 3 == 0) return false;
    if (num < 25) return true;

    for (i = 5; i <= end; i += 6)
        if (num % i == 0 || num % (i + 2) == 0)
            return false;

    return true;

}

long long* step(int step, long long start, long long end) {

    long long i;
    long long *result = calloc (2 , sizeof (long long));

    for (i = start; i <= end; ++i)
        if (is_prime (i) && is_prime (i + step)) {
            result[0] = i;
            result[1] = i + step;
            return result;
        }

    return result;
}
