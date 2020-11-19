#include <stdbool.h>

bool is_prime(int num) {

    const unsigned end = sqrt (num);

    if (num <= 3) return num > 1;
    if (num % 2 == 0 || num % 3 == 0) return false;

    for (unsigned i = 5; i <= end; i += 6)
        if (num % i == 0 || num % (i + 2) == 0)
            return false;

    return true;
}
