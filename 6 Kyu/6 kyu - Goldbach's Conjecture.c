#include <stdio.h>

#include <stdbool.h>
#include <math.h>

bool is_prime (unsigned num) {

    const unsigned end = sqrt (num);

    if (num <= 3) return num > 1;
    if (num % 2 == 0 || num % 3 == 0) return false;

    for (unsigned i = 5; i <= end; i += 6)
        if (num % i == 0 || num % (i + 2) == 0)
            return false;

    return true;
}

void check_goldbach (unsigned n, unsigned *a, unsigned *b) {

    if (n < 2 || n % 2 != 0) return ;

    for (unsigned i = 2; i < n; i++) {
        if (is_prime (i)) {
            for (unsigned j = n - 1; j >= i; j--) {
                if (is_prime (j) && i + j == n) {
                    *a = i, *b = j;
                    return;
                }
            }
        }
    }

    return ;
}

void testn (int n) {

    int a = 0, b = 0;
    check_goldbach (n, &a, &b);

    printf ("%d %d\n", a, b);
}

int main () {

    testn (14);
}
