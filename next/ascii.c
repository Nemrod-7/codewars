#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>

int mod3 (int n) {

    if (n < 0) return ((n %= 3) < 0) ? n + 3 : n;
    if (n * 0xAAAAAAAB <= n) return 0;
    return n % 3;
}
int lucas3 (int n, int k) {

    int prod = 1, np, kp;
    // => lucas (n k) = prod from i = 0 to infinity (binom(n_i k_i) mod p)
    do {
        np = mod3 (n), kp = mod3 (k);
        //cout << '(' << np << ' ' << kp << ") => " << binomial (np, kp) << endl;
        if (np < kp) return 0;
        if (np == 2 && kp == 1) prod *= 2; //prod *= binomial (np, kp);

        n /= 3, k /= 3;

    } while (n || k);


    return mod3 (prod);
}

void printcantor (double x, double y) {

    for (double i = x; i < y; i += 0.02) {
        printf ("#");
    }

    const double mid = (y - x) * 0.5;
    assert (mid != 0.0);

    printcantor (x, mid);
    for (double i = mid; i < y - mid; i += 0.02) {
        printf (" ");
    }
    printcantor (y - mid, y);
    printf ("\n");
}

bool cantor (double x, double y, int limit, int cycle) {

    if (cycle >= limit) return true;
    const double mid = (y - x) / 3.0;

    printf ("[%f,%f] U [%f,%f] => %d\n", x, mid, y - mid, y);
    assert (mid != 0.0);

    bool left = cantor (x, mid, limit, cycle + 1);
    bool right = cantor (y - mid, y, limit, cycle + 1);

    if (left && right) {
      return true;
    }
}




int main () {

    int num = 1, den =  3;
    int n = 100; // => return True

    double x = 0, y = 1;
    const double mid = (y - x) / 3.0;

    double rd = num / (double) den;
    //printf ("[%f,%f] U [%f,%f] => %d\n", x, mid, y - mid, y);
    /*
    for (int n = 0; n <= size; ++n) {
        const int pad = (size - n) * 2;

        for (int i = 0; i <= pad; ++i) {
            printf (" ");
        }

        for (int k = 0; k <= n; ++k) {
            int cell = lucas3 (n, k);

            if (cell == 0) {
                printf ("    ");
            } else {
                printf ("  . ");
            }
        }
        printf ("\n");
    }
    */
    printf ("end");
}
