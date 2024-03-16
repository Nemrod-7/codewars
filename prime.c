#include <stdio.h>

int twin_prime(int n) {
    int lim = n + 2, cnt = 0;
    _Bool sieve[100] = {};

    for (int i = 3; i * i <= lim; i++) {
        if (sieve[i] == 0) {
            for (int j = i * i; j <= n; j += 2 * i) {
                sieve[j] = 1;
            }
        }
    }

    for (int i = 3; i < lim -2 ; i += 2) {
        if (sieve[i] == 0 && sieve[i+2] == 0) {
            cnt++;
        }
    }
    return cnt;
}

int main () {

    int cnt = twin_prime(12);

    int factorial[20];
    factorial[0] = 1;

    for (int i = 1; i < 10; i++) {
        factorial[i] = factorial[i-1] * i;
    }


}
