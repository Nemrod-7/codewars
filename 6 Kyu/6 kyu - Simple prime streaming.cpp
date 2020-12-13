#include <cmath>

inline bool is_prime (uint64_t num) {
    if (num < 4) return true;
    if ((num &1) == 0) return false;
    if (num % 3 == 0) return false;

    const uint64_t end = sqrt (num);

    for (uint64_t i = 5; i <= end; i += 6)
        if (num % i == 0 || num % (i + 2) == 0)
            return false;

    return true;
}
std::string solve(int a, int b) {
    std::string os;
    int num = 2;
    size_t end = a + b;

    while (os.size() < end) {
        if (is_prime (num))
            os += std::to_string (num);
        num++;
    }

    return os.substr (a, b);
}
