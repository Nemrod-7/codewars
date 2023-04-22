#include <iostream>
#include <vector>

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

double distance (const Point &a, const Point &b) { /* magnitude of point AB */ return rnd (hypot (a.x - b.x, a.y - b.y)); }


int main () {

    /*
    Your arc circle's midpoint has an angle from its midpoint that points towards the other circle.
    Find the closest angle to that angle that lies within the arc, and project to the appropriate point on the arc.
    If that point lies within the circle, then you know your arc intersects the circle.
    */

    Point start = {-3, 1}, exit = {4.25, 0};
    vector<Circle>  graph = {{0.0, 0.0, 2.5}, {1.5, 2.0, 0.5}, {3.5, 1.0, 1.0}, {3.5, -1.7, 1.2}};
    vector<Point> edge = {{0.000000 2.500000},{2.400000 0.700000},{2.122642 1.320755},{2.500000 -0.000000},{-0.278174 -2.484476},{2.476399 -0.342708},{1.800352 -1.734570}};
    vector<Point> stan {{-2.359123 -0.827369},{-1.390877 2.077369}};

    int id = 0;
    Point a = stan[id], b = edge[0];

    double op = distance (a,b) / 2.0;

}
