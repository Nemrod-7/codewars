#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

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

const double pi = M_PI;

/*
   trigonometry spreadsheet => theta is angle between hypothenus and adjacent size
   Soh cah toa

   theta = asin (opp / hyp); soh
   theta = acos (adj / hyp); cah
   theta = atan (opp / adj); toa

   Quadrant         Angle         sin   cos   tan
   -------------------------------------------------
   I           0    < α < π/2      +     +     +
   II          π/2  < α < π        +     -     -
   III         π    < α < 3π/2     -     -     +
   IV          3π/2 < α < 2π       -     +     -

   theta in radian = deg * pi / 180
   angle in degree = rad * 180 / pi

            a = {cx + rad . cos(theta), cy + rad . sin(theta)}
           /|
         /  |
hyp    /    |  adj
     /     _|
  ./______|_|____angle in degree
  c   opp

  rotate : nx = x * cos(th) - y * sin(th)  ny = x * sin(th) + y * cos(th)

               90° x = sin(th), y = -cos(th)
               |
               |
               |
  180° ---------+--------> 0-360° x = cos(th), y = sin(th)
  x = -cos(th) |
  y = -sin(th) |
               |
               |
              270° x = -sin(th), y = cos(th)

  forward  : x = +cos(th), y = +sin(th)
  backward : x = -cos(th), y = -sin(th)
  left     : x = +sin(th), y = -cos(th)
  right    : x = -sin(th), y = +cos(th)
 */

double degree (double rad) { return rad * 180.0 / pi; }
double rnd (const double x) { return round (x * 1e8) / 1e8; }
double magnitude (const Point &a, const Point &b) { return hypot (a.x - b.x, a.y - b.y); }

int main () {

    Circle c1 ({0.0, 0.0}, 1.5);
    auto [p1,r1] = c1;

    Point a (0.0 , 1.5), b = {1.5, 0.0}; // two points on the circle

    double perimeter = 2.0 * pi * r1;

    const double hyp = r1, opp = magnitude (a,b) * 0.5;

    const double theta = asin (opp / hyp) * 2.0;
    const double arc = r1 * theta;

    const double inv = r1 * (2 * pi - theta);

    double x2 = p1.x - (a.x - p1.x), y2 = p1.y - (a.y - p1.y); // inverse point



}
