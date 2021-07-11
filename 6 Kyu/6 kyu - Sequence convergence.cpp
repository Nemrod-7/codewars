#include <iostream>

void next (int &n) {
    int num = n, mul = 1;

    while (num) {
        if (num % 10 != 0) mul *= num % 10;
        num /= 10;
    }
    n += mul;
}
int convergence (int n) {

    int cnt = 0, base = 1;

    while (n != base) {

        if (n > base)
            next (base);
        else {
            next (n);
            cnt++;
        }
    }

    return cnt;
}
