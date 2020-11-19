#include <cmath>

static int pointsNumber(int radius){
    const double r2 = pow (radius, 2);
    int i, sum = 0;

    for (i = 0; i < radius; ++i)
        sum += sqrt (r2 - pow (i, 2));

    return 4 * sum + 1;
}
