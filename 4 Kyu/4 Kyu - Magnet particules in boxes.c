#include <math.h>

double v (int k, int n) {
    return 1 / (k * pow ((n + 1), 2 * k));
}
double u (int k, int N) {
    double sum = 0; for (int n = 1; n <= N; ++n) sum += v (k,n);
    return sum;
}
double doubles (int maxk, int maxn) {
    double sum = 0; for (int k = 1; k <= maxk; ++k) sum += u (k,maxn);
    return sum;
}
