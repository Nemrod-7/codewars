#include <iostream>
#include <vector>
#include <cmath>

using point = std::pair<double,double>;

double radian (const double x) { return x * M_PI / 180; }
double distance (const point &a, const point &b) { return std::hypot (b.first - a.first, b.second - a.second); }
point getnext (point p, double dist, double angle) {
    auto [dx,dy] = p;
    double nx = dx + dist * cos (radian (angle)) ;
    double ny = dy + dist * sin (radian (angle)) ;

    return {nx, ny};
}

std::vector<int> solve (double a, double b, double c, double alpha, double beta, double gamma) {

    double dx = a * cos (radian (alpha)) + b * cos (radian (90 + beta)) + c * cos (radian (180 + gamma));
    double dy = a * sin (radian (alpha)) + b * sin (radian (90 + beta)) + c * sin (radian (180 + gamma));
    double angle = atan2 (dy, dx) * 180 / M_PI;
    printf ("%f %f\n",a * cos (radian (alpha)), a * sin (radian (alpha)));
    int dist = round (std::hypot (dx,dy));

    int deg = floor (angle);
    double min = ((angle - deg) * 60);
    int sec = floor ((min - floor (min)) * 60);
    // printf ("%i => ", dist);
    // printf ("%i %.0f %i", deg, min, sec);
    return {dist, deg, static_cast<int>(min), sec};
}
int main () {

    solve (12, 20, 18, 45, 30, 60);

}
