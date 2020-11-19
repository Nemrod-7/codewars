// Pi (not in math.h)
#define M_PI 3.14159265358979323846
#include <math.h>

_Bool is_valid (double a, double b) { return a != 0 && b != 0;}

double missingAngle(double h, double a, double o) {
    const double deg = 180 / M_PI;
  //double theta = 0;
  //double soh = o / h, cah = a / h, toa = o / a;
    if (is_valid (o, h)) return roundf (asin (o / h) * deg);
    if (is_valid (a, h)) return roundf (acos (a / h) * deg);
    if (is_valid (o, a)) return roundf (atan (o / a) * deg);
}
