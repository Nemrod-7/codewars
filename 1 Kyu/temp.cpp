#include <iostream>
#include <cmath>
#include <vector>
#include <limits>

using namespace std;

struct Point {
    double x,y;
    Point (double x = 0.0, double y = 0.0) : x(x), y(y) {}
};
struct Circle {
    Point ctr;
    double r;

    Circle () : ctr(), r(1.0) {}
    Circle (Point c, double r) : ctr(c), r(r) {}
    Circle (double cx, double cy, double r) : ctr(cx,cy), r(r) {}
};

double sq (const double x) { return x * x; }
double magnitude (const Point &a, const Point &b) {  return hypot (a.x - b.x, a.y - b.y); }
double arclength (const Point &a, const Point &b, const Circle &c1) { // arc length of circle segment a-b
    const auto &[c,r] = c1;
    const double ab = magnitude (a,b) * 0.5, hyp = magnitude (a,c); // ab -> opposite side, hyp = hypthenuse
    return r * asin (ab / hyp);
}

vector<Point> circles_intersection (const Circle &c1, const Circle &c2) { // intersection points of 2 circles
    const auto &[p1,r1] = c1;
    const auto &[p2,r2] = c2;
    const double dist = magnitude (p1, p2);

    if (dist <= r1 + r2 && dist >= fabs (r2 - r1)) {
        const double ex = (p2.x - p1.x) / dist;
        const double ey = (p2.y - p1.y) / dist;

        const double a = (sq(r1) - sq(r2) + sq(dist)) / (2 * dist);
        const double h = sqrt (r1 * r1 - a * a);

        const Point p3 = {p1.x + a * ex - h * ey, p1.y + a * ey + h * ex};
        const Point p4 = {p1.x + a * ex + h * ey, p1.y + a * ey - h * ex};
        return {p3,p4};
    }
    return {};
}


int main () {

    /*
    Your arc circle's midpoint has an angle from its midpoint that points towards the other circle.
    Find the closest angle to that angle that lies within the arc, and project to the appropriate point on the arc.
    If that point lies within the circle, then you know your arc intersects the circle.
    */

    Point start = {-3, 1}, exit = {4.25, 0};
    vector<Circle>  graph = {{0.0, 0.0, 2.5}, {1.5, 2.0, 0.5}, {3.5, 1.0, 1.0}, {3.5, -1.7, 1.2}};
    vector<Point> edge = {{0.000000,2.500000},{2.400000,0.700000},{2.122642,1.320755},{2.500000,-0.000000},{-0.278174,-2.484476},{2.476399,-0.342708},{1.800352,-1.734570}};
    vector<Point> stan {{-2.359123,-0.827369},{-1.390877,2.077369}};

    int id = 0;
    Circle c1 = graph[id], c2 = graph[1];

    Point a = stan[id], b = edge[0];
    double hy = magnitude (a, c1.ctr);

    double minv = numeric_limits<double>::max();

    for (auto p : circles_intersection (c1,c2)) {
        minv = min (minv, arclength (a,p,c1));
    }

    double ab = magnitude (a,b) * 0.5;
    double theta = 2 * asin (ab / hy) * 180.0 / M_PI;
    double arc = 2 * M_PI * c1.r * (theta / 360.0);

    printf ("%f \n", theta);
}
