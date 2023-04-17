#include <iostream>
#include <cmath>

using namespace std;

struct Point { double x, y; };

double slope (const Point &a, const Point &b) { return (b.y - a.y) / (b.x - a.x); }
double radian (const double deg) { return deg * M_PI / 180; }
double distance (const Point &a, const Point &b) { return hypot (b.y - a.y, b.x - a.x); }

double gety (double x, pair<double,double> eq) { return eq.first * x + eq.second; }
double getx (double y, pair<double,double> eq) { return (y - eq.second) / eq.first; }

int main () {


    double angle = 60.0, h = 0.5, length = 1.8;

    double t = 0.01;
    pair<double,double> up {0.0,1.0}, down {0.0,0.0};

    double m = tan (radian(angle));
    double ycept = h;
    /*
    1st bounce happens at (0.2887, 1) with total length 0.5773
    2nd bounce happens at (0.8660, 0) with total length 1.7320
    3rd bounce doesn't happen because the ray runs hits the length before hitting the wall, at (0.9,0.0588)

    y = m * x + ycept;
    x = (y - ycept) / m;

    */

    Point p;
    pair<double,double> line = {m,ycept};

    double x = (1.0 - ycept) / m;
    double y = m * x + ycept;

    // 1 = m * x + ycept
    cout << x << " " << y << " :: " << distance ({0,h},{x,y}) << "\n";

    for (double i = 0; i < length; i += t) {

    }


}
