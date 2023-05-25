#include <iostream>
#include <cmath>
#include "base.hpp"

using namespace std;

int gcd (int a, int b) { return b == 0 ? a : gcd (b, a % b); }

double descartes (int64_t r1, int64_t r2) {
    double c1 = r1 * r2;
    double c2 = r1 + r2 + 2 * sqrt (c1);
    double c3 = c1 / c2;

    return c3;
}
int64_t trinum (int64_t n) { // triangular number
    return n * (n + 1) / 2;
}
int64_t problem510 (int n) {
    int64_t sum = 0;

    for (int64_t i = 1; i * i <= n; i++) {
        int64_t c1 = i * i;
        for (int64_t j = 1; j <= i; j++) {
            int64_t c2 = j * j;

            int64_t num = c1 * c2, den = (i + j) * (i + j);
            int64_t c3 = num / den ;
            if (c3 * den != num) continue;


            if (gcd (gcd (c1,c2), c3) == 1) {
                int64_t cnt = c1 + c2 + c3;
                sum += (c1 + c2 + c3) * trinum (n / c1);
                // cout << c1 << " " << c2 << " " << c3 << " " << trinum (n / c1) << "\n";
            }
        }
    }
    // cout << sum;
    return sum;
}


int main () {

    Timer chrono;

    int64_t n = 1000000000;
    // int64_t n = 225; // 3072
    for (int64_t i = 1 ; i <= n; i *= 10) {
      cout << i << " :: " << problem510 (i) << "\n" ;
    }
        // cout << problem510(n) << "\n" ;


    // problem510 (100);
    chrono.stop();
    chrono.get_duration();
}
