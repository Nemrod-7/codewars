#include <cmath>

unsigned middle_point (double x1, double y1, double z1,
                       double x2, double y2, double z2,
                       double x3, double y3, double z3) {

    double thr = (std::hypot (x1 - x2, y1 - y2, z1 - z2));
    double scd = (std::hypot (x1 - x3, y1 - y3, z1 - z3));
    double fst = (std::hypot (x2 - x3, y2 - y3, z2 - z3));
    
    if (int(fst - (scd + thr)) == 0) return 1;
    if (int(scd - (fst + thr)) == 0) return 2;
    if (int(thr - (fst + scd)) == 0) return 3;

    return 0;
}
