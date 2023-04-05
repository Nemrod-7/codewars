#include <iostream>
#include <iomanip>
#include <random>

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

bool operator == (const Point &a, const Point &b) { return a.x == b.x && a.y == b.y; }
bool operator != (const Point &a, const Point &b) { return a.x != b.x || a.y != b.y; }
bool operator < (const Point &a, const Point &b) { return make_pair (a.x,a.y) < make_pair (b.x,b.y); }

double rnd (const double x) { return round (x * 1e8) / 1e8; }
double distance (const Point &a, const Point &b) { return rnd (hypot (a.x - b.x, a.y - b.y)); }
bool inside_circle (const Point &p, const Circle &c) { return distance (c.ctr, p) < c.r; }

bool is_valid (const Point &p, const vector<Circle> &space) {

    for (auto &cir : space) {
        if (inside_circle (p, cir) == true) {
            return false;
        }
    }
    return true;
}

class Display {
    public :

        static void point (const Point &pt) {
            cout << fixed << setprecision(12) << "[" << pt.x << " , " << pt.y << "]\n";
        }
        static void vect (const vector<Point> &vect) {
            for (auto pt : vect) {
                point (pt);
            }
        }
};

void Test () {
    const double max_error = 1e-8;
  Point a,b;
  vector<Circle> c;
  double actual, expected;

  //It(Example_In_The_Picture)
  {
    a = {-3, 1};   b = {4.25, 0};
    c = { {0.0, 0.0, 2.5}, {1.5, 2.0, 0.5}, {3.5, 1.0, 1.0}, {3.5, -1.7, 1.2} };
    // actual = shortest_path_length(a,b,c);
    // Assert::That(actual, EqualsWithDelta(9.11821650244, max_error));
  }

  ///It(Long_Way_Round)
  {
    a = {0, 1};  b = {0, -1};
    c = { {0.0, 0.0, 0.8}, {3.8, 0.0, 3.2}, {-3.5, 0.0, 3.0}, {-7.0, 0.0, 1.0} };
    // actual = shortest_path_length(a,b,c);
    // Assert::That(actual, EqualsWithDelta(19.0575347577, max_error));
  }

  // It(Straight_Line)
  {
    a = {3, 0};   b = {0, 4};
    c = { {0, 0, 1} };
    // actual = shortest_path_length(a,b,c);
    // Assert::That(actual, EqualsWithDelta(5.0, max_error));
  }

  /// It(No_Way_Through)
  {
    a = {0, 0};  b = {20, 20};
    c = { {4, 0, 3}, {-4, 0, 3}, {0, 4, 3}, {0, -4, 3} };
    // actual = shortest_path_length(a,b,c);
    // Assert::That(actual, Equals(-1.0));
  }

  //  It(Other_Simple_Tests)
  {
    a = {0, 1};  b = {0, -1};
    c = { {0.0, 0.0, 0.8}, {-3.8, 0.0, 3.2}, {3.5, 0.0, 3.0}, {7.0, 0.0, 1.0} };
    // actual = shortest_path_length(a,b,c);
    // Assert::That(actual, EqualsWithDelta(19.0575347577, max_error));

    a = {0, -7};  b = {8, 8};
    c = { };
    // actual = shortest_path_length(a,b,c);
    // Assert::That(actual, EqualsWithDelta(17.0, max_error));

    a = {-3.5, 0.1};  b = {3.5, 0.0};
    double r = 2.01;
    c = { {0,0,1}, {r, 0.0, 1.0}, {r*0.5, r*sqrt(3)/2, 1.0}, {-r*0.5, r*sqrt(3)/2, 1.0},
                  {-r, 0.0, 1.0}, {r*0.5, -r*sqrt(3)/2, 1.0}, {-r*0.5, -r*sqrt(3)/2, 1.0} };
    // actual = shortest_path_length(a,b,c);
    // Assert::That(actual, EqualsWithDelta(7.80758072818, max_error));

    // start in forbidden zone:
    a = {0.5, 0.5};  b = {2, 2};
    c = { {0, 0, 1} };
    // actual = shortest_path_length(a,b,c);
    // Assert::That(actual, Equals(-1.0));

    // end in forbidden zone:
    a = {2, 2};  b = {0.5, 0.5};
    c = { {0, 0, 1} };
    // actual = shortest_path_length(a,b,c);
    // Assert::That(actual, Equals(-1.0));
  }
}
