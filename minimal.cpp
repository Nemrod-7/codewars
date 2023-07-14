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

/*

   trigonometry spreadsheet => theta is angle between hypothenus and adjacent size
   Soh cah toa

   theta = asin (opp / hyp);
   theta = acos (adj / hyp);
   theta = atan (opp / adj);


   Quadrant    Angle              sin   cos   tan
   -------------------------------------------------
   I           0    < α < π/2      +     +     +
   II          π/2  < α < π        +     -     -
   III         π    < α < 3π/2     -     -     +
   IV          3π/2 < α < 2π       -     +     -

*/

double degree (double rad) { return rad * 180.0 / M_PI; }
double rnd (const double x) { return round (x * 1e8) / 1e8; }
double magnitude (const Point &a, const Point &b) { return hypot (a.x - b.x, a.y - b.y); }

void problem510 (int n) {

    for (double i = 0; i <= n; i++) {
        double c1 = 1 / sqrt (i);
        for (int j = i; j <= n; j++) {
            double c2 = 1 / sqrt (j);
            double c3 = c1 + c2;

            cout << i << " " << j << " :: " << c3 << "\n";
        }
    }

}
int main () {

    Circle c1 ({0.0, 0.0}, 1.5);
    auto [p1,r1] = c1;

    Point a (0.0 , 1.5), b = {1.5, 0.0};

    double perimeter = 2.0 * M_PI * r1;

    const double hyp = r1, opp = magnitude (a,b) * 0.5;

    const double theta = asin (opp/hyp) * 2.0;
    const double arc = r1 * theta;

    const double inv = r1 * (2 * M_PI - theta);

    double x2 = p1.x - (a.x - p1.x), y2 = p1.y - (a.y - p1.y); // inverse point



    int n = 5;

    problem510(n);

}
