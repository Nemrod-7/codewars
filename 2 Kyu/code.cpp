#include <vector>
#include <map>
#include <list>
#include <cmath>
#include <algorithm>

struct Point {
    double x,y;

    Point() : x(0.0), y(0.0) {}
    Point(double x, double y) : x(x), y(y) {}
};

double area (const std::vector<Point> &geom) { // area of a convex polygon :: all the point must be ordered clockwise
    double area = 0;

    for (size_t i = 0; i < geom.size(); i++) {
        size_t j = (i + 1) % geom.size();
        area += ((geom[i].x * geom[j].y) - (geom[j].x * geom[i].y));
    }

    return fabs (area) * 0.5;
}


int main () {
  
  Point seed[] = {{-1.049,4.626},{1.024,-3.809},{-4.049,-0.776},{-2.348,4.552},{2.094,-4.806},{-3.240,0.227},{3.575,-3.069},{-1.497,2.294},{-3.876,1.089},
      {1.491,-3.934},{4.817,3.037},{-3.551,-1.305},{1.023,3.013},{2.418,-4.567},{4.432,3.426}};
}
