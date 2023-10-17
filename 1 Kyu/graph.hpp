#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>

struct Point {
    double x,y;

    Point() : x(0.0), y(0.0) {}
    Point(double x, double y) : x(x), y(y) {}
};
struct Circle {
    Point ctr;
    double r;

    Circle() : ctr(), r(1.0) {}
    Circle(Point c, double r) : ctr(c), r(r) {}
    Circle(double cx, double cy, double r) : ctr(cx,cy), r(r) {}
};

struct {
  bool operator () (const Point &a, const Point &b) const {
      return a.x != b.x ? a.x < b.x : a.y < b.y;
  }
} verticmp;

struct {
  bool operator () (const Point &a, const Point &b) const {
      return a.y != b.y ? a.y > b.y : a.x > b.x;
  }
} horicmp;

const double epsilon = 1e-8, pi = 3.14159265358979323846;
const double radian = pi / 180.0, degree = 180.0 / pi;

bool operator == (const Point &a, const Point &b) { return a.x == b.x && a.y == b.y; }
bool operator < (const Point &a, const Point &b) { return std::make_pair (a.x,a.y) < std::make_pair (b.x,b.y); }
bool operator != (const Point &a, const Point &b) { return a.x != b.x || a.y != b.y; }

// double rnd (const double x) { return round (x * 1e8) / 1e8; }
double sq (const double x) { return x * x; }
double slope (const Point &a, const Point &b) { return (a.x != b.x) ? (b.y - a.y) / (b.x - a.x) : 1e8; }
double distance (const Point &a, const Point &b) { return std::hypot (a.x - b.x, a.y - b.y); }
double intercept (const Point &a, const double m) { return a.y - (m * a.x); }
double angle (const Point &p1, const Point &p2, const Circle &c1) { // central angle of a circle
    return 2.0 * asin (distance (p1,p2) * 0.5 / c1.r);
}
bool direction (const Point &a, const Point &b, const Point &c) { return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x) > 0.0; }

////////////////////////////////////////////////////////////////////////////////
double ccw (const Point &o, const Point &a, const Point &b) { return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x); }
double area (const std::vector<Point> &geom) { // area of a convex polygon :: all the point must be ordered clockwise
    double area = 0;

    for (size_t i = 0; i < geom.size(); i++) {
        size_t j = (i + 1) % geom.size();
        area += ((geom[i].x * geom[j].y) - (geom[j].x * geom[i].y));
    }

    return abs (area) * 0.5;
}
Point barycentre (const Point &p1, const Point &p2, const Point &p3) { // get triance centroid
    double x = (p1.x + p2.x + p3.x) / 3;
    double y = (p1.y + p2.y + p3.y) / 3;
    return {x, y};
}
Point circumcenter (const Point &a, const Point &b, const Point &c) { // get ttriangle circumcenter
    const double acx = a.x - c.x, bcy = b.y - c.y, bcx = b.x - c.x, acy = a.y - c.y;
    const double D = acx * bcy - bcx * acy;
    Point p;

    p.x = ((acx * (a.x + c.x) + acy * (a.y + c.y)) / 2 * bcy - (bcx * (b.x + c.x) + bcy * (b.y + c.y)) / 2 * acy) / D;
    p.y = ((bcx * (b.x + c.x) + bcy * (b.y + c.y)) / 2 * acx - (acx * (a.x + c.x) + acy * (a.y + c.y)) / 2 * bcx) / D;

    return p;
}
std::vector<Point> convex_hull (std::vector<Point> curr) {

    const int size = curr.size();
    int i, t, k = 0;
    std::vector<Point> hull (size * 2);
    std::sort (begin(curr), end(curr), verticmp);

    for (i = 0; i < size; i++) {
        while (k >= 2 && ccw (hull[k - 2], hull[k - 1], curr[i]) <= 0)
            k--;

        hull[k++] = curr[i];
    }

    for (i = size - 2, t = k + 1; i >= 0; i--) {
        while (k >= t && ccw (hull[k - 2], hull[k - 1], curr[i]) <= 0)
            k--;

        hull[k++] = curr[i];
    }

  //return vector<Point>(h.begin(), h.begin() + k - (k > 1));
    hull.resize (k - 1);
    return hull;
}
int nearest_point (const Point &a, const std::vector<Point> &graph) {

  double minv = std::numeric_limits<double>::infinity(), dist;
  int near;

  for (size_t i = 0; i < graph.size(); i++) {
    dist = distance (a, graph[i]);

    if (a != graph[i] && dist < minv) {
      near = i;
      minv = dist;
    }
  }

  return near;
}

///////////////////////////////// circles //////////////////////////////////////
bool inside_circle (const Point &p, const Circle &c) { return (c.r - distance (c.ctr, p)) > epsilon; }
std::vector<Point> tangent (const Point &p1, const Circle &c) { // tangent points of a cricle => gives two lines p1,t1 and p1,t2
    const auto &[c1,rad] = c;
    const double theta = acos (rad / distance (p1,c1)); // angle == adj / hyp
    const double direc = atan2 (p1.y - c1.y, p1.x - c1.x); // direction angle of point P from C

    const double d1 = direc + theta, d2 = direc - theta;
    const Point t1 = {c1.x + rad * cos (d1), c1.y + rad * sin (d1)}; // tangent to circle c1 => line : p1 ~ p2
    const Point t2 = {c1.x + rad * cos (d2), c1.y + rad * sin (d2)}; // tangent to circle c1 => line : p1 ~ p2

    return {t1,t2};
}
std::vector<Point> interception (const Point &p1, const Point &p2, const Circle &c) { // interception points of a line (p1,p2) and a circle
    const auto &[c1,r1] = c;
    const Point p3 = {p1.x - c1.x, p1.y - c1.y}, p4 = {p2.x - c1.x, p2.y - c1.y}; //shifted line points

    const double m = slope (p3,p4);    // slope of the line
    const double b = p3.y - m * p3.x;                  // y intercept of line
    const double np = sq(r1) * sq(m) + sq(r1) - sq(b); // quad equation

    if (np > epsilon) {
        const double t1 = (-m * b + sqrt(np)) / (sq(m) + 1);
        const double t2 = (-m * b - sqrt(np)) / (sq(m) + 1);

        const Point i1 = {t1 + c1.x, m * t1 + b + c1.y};
        const Point i2 = {t2 + c1.x, m * t2 + b + c1.y};
        return {i1,i2};
    }

    return {};
}
std::vector<Point> circles_intersection (const Circle &c1, const Circle &c2) { // intersection points of 2 circles
    const auto &[p1,r1] = c1;
    const auto &[p2,r2] = c2;
    const double dist = distance (p1, p2);

    if (dist <= r1 + r2 && dist >= abs (r2 - r1)) {
        const double dx = (p2.x - p1.x) / dist;
        const double dy = (p2.y - p1.y) / dist;

        const double a = (sq(r1) - sq(r2) + sq(dist)) / (2 * dist);
        const double h = sqrt (r1 * r1 - a * a);

        const Point p3 = {p1.x + a * dx - h * dy, p1.y + a * dy + h * dx};
        const Point p4 = {p1.x + a * dx + h * dy, p1.y + a * dy - h * dx};
        return {p3,p4};
    }
    return {};
}
std::vector<Point> hcenter (const Circle &c1, const Circle &c2) { // homothetic centers of two circles
    const auto &[p1,r1] = c1;
    const auto &[p2,r2] = c2;
    const double mn = r1 - r2, mx = r1 + r2;
    Point h1, h2; // : 1 => external, 2 => internal
    std::vector<Point> hc;

    if (c1.r == c2.r) { // if 2 circles have the same radius, the external hc == inf. so 2 external hc must be determined perpendicular to the midpoint
        const Point c = {(p2.x + p1.x) * 0.5, (p2.y + p1.y) * 0.5};
        const double theta = atan2 (p2.y - p1.y, p2.x - p1.x);

        h1.x = c.x - r1 * sin (theta), h1.y = c.y + r1 * cos (theta);
        hc.push_back (h1);
        h1.x = c.x + r1 * sin (theta), h1.y = c.y - r1 * cos (theta);
        hc.push_back (h1);
    } else {
        h1.x = -r2 / mn * p1.x + r1 / mn * p2.x;
        h1.y = -r2 / mn * p1.y + r1 / mn * p2.y;
        hc.push_back (h1);
    }

    h2.x = r2 / mx * p1.x + r1 / mx * p2.x;
    h2.y = r2 / mx * p1.y + r1 / mx * p2.y;
    hc.push_back (h2);

    return hc;
}
bool collision (const Point &p1, const Point &p2, const std::vector<Circle> &circles) {
    const double dist = distance (p1,p2);

    for (const Circle &c1 : circles) {
        if (inside_circle (p1, c1) || inside_circle (p2, c1)) {
            return true;
        }
        std::vector<Point> icp = interception (p1, p2, c1);
        for (auto &ip : icp) { // check if a circle lies between the line (p1,p2)
            if ((distance (p1, ip) + distance (ip, p2) - dist) <= epsilon) {
                return true;
            }
        }
    }

    return false;
}

////////////////////////////////// Parabolas ///////////////////////////////////
std::vector<double> quadratic (const double a, const double b, const double c) {
    const double delta = b * b - 4 * a * c;
    const double square = sqrt (delta);
    const double x1 = -(b + square) / (2.0 * a), x2 = -(b - square) / (2.0 * a);
    // const double x3 = c / (a * x1), x4 = c / (a * x2); // floating point approximation
    return {x1, x2};
}
std::vector<double> parabola (const Point &p, const double sweep) {
    const double d = 2.0 * (p.y - sweep);

    const double a = 1.0 / d;
    const double b = -2.0 * p.x / d;
    const double c = p.x * p.x / d + d / 4.0 + sweep;
    // cout << "a : " << arc0[0] << " b : " << arc0[1] << " c : " << arc0[2] << endl;
    return {a,b,c};
}
std::vector<double> parabola_intersect (const std::vector<double> &a0, const std::vector<double> &a1) {
    return quadratic(a0[0] - a1[0], a0[1] - a1[1], a0[2] - a1[2]);
}

////////////////////////////////////////////////////////////////////////////////
class Show {
    public :
        static void point (const Point &pt) {
            std::cout << std::fixed << std::setprecision(3) << "[" << pt.x << " , " << pt.y << "]";
        }
        static void vect (const std::vector<Point> &vect) {
            for (auto pt : vect) {
                point (pt);
            }
            std::cout << "\n";
        }
};

std::vector<Point> generatepoints (int size) {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> rnd (-5, 5);

    std::vector<Point> graph;

    while (size-->0) {
        graph.push_back ({rnd(gen), rnd(gen)});
    }

    return graph;
}
double clamp (double x, double mini, double maxi) { // limit a position to an area
  if (x < mini) x = mini;
  if (x > maxi) x = maxi;
  return x;
}
