#include <math.h>

/*
    DERIVATION

    U         = x + 2x^2 + 3x^3 + ... +       nx^n
    Ux        =     x^2  + 2x^3 + ... + (n - 1)x^n + nx^(n + 1)
    -----------------------------------------------------------
    U(1 - x)  = x + x^2  + x^3  + ... +        x^n - nx^(n + 1)
    U(x - 1)  = nx^(n + 1) - (x^(n + 1) - x)/(x - 1)
    U         = nx^(n + 1) /(x - 1) - (x^(n + 1) - x)/(x - 1)^2
              = (n(x - 1) x^(n + 1) - x^(n + 1) + x)/(x - 1)^2

    U (n, x) = sum from x = 0 to infinity (nx^x) == x / (x - 1)^2

    The limit as n -> infinity is equal to m.

    lim U   m = x/(x - 1)^2
            x = m(x^2 - 2x + 1)
            mx^2 - (2m + 1)x + m = 0

    x = (2m + 1 - sqrt (4m + 1)) / 2m
*/

double solve (double m) { return (2 * m - sqrt (4 * m + 1) + 1 ) / (2 * m); }

int main () { return 0;}
