#include <cmath>

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

class StepInPrimes {
    public:
        static std::pair <long long, long long> step(int step, long long start, long long end) {

            for (long long i = start; i <= end; ++i)
                if (is_prime (i) && is_prime (i + step))
                    return {i, i + step};

            return {};
        }
};
