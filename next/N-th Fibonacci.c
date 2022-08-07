// N-th Fibonacci
#include <stdio.h>

int main () {

    int nth = 100;

    unsigned long long f1 = 0, f2 = 1, f3 = 1;

    for (int i = 0; i < nth; i++) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
        printf ("%i, ",f3);
    }

}
