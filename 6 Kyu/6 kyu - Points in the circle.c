#include <math.h>

int pointsNumber(int radius) {
    const double r2 = radius * radius;
    double sum = 0;

    for (int i = 0; i <= r2; ++i) {
        sum += floor (r2 / (4 * i + 1)) - floor (r2 / (4 * i + 3));
    }

    return 1 + sum * 4;
}
