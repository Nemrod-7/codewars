#include <iostream>

long nextBigger (long n) {

    std::string num = std::to_string (n);

    const size_t len = num.size();
    size_t up = len, pos = len - 1;

    while (pos-->0) {
        if (num[pos] < num[pos + 1]) break;
        if (pos == 0) return -1;
    }

    if (pos == -1) return -1;

    while (up-->0)
        if (num[up] > num[pos])
            break;

    std::swap (num[pos], num[up]);

    for (size_t i = pos + 1; i < len; ++i)
        for (size_t j = i + 1; j < len; j++)
            if (num[i] > num[j])
                std::swap(num[i], num[j]);

    return std::stol(num, nullptr, 10);
}

int main () {


}
