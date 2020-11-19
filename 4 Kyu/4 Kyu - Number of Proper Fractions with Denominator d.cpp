#include <cmath>

long properFractions(long n) {

    if (n == 1) return 0;

    long res = n, carry = n, i = 2;
    for (; i <= sqrt(carry); ++i) {
       if (carry % i == 0) {
            res = res / i * (i - 1);
            while (carry % i == 0)
                carry /= i;
       }
    }

    if (carry>1)
        res = res / carry * ( carry-1);

  return res;
}
