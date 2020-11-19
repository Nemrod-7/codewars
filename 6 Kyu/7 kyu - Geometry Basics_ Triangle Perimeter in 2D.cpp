#include <cmath>

double dist (const Point p, const Point q) {
    double x = p.x - q.x, y = p.y - q.y;
    return sqrt (x * x + y * y);
}

double triangle_perimeter(const Triangle& t) {
  return dist (t.a, t.b) + dist (t.b, t.c) + dist (t.c, t.a);
}
