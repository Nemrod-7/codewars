#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <map>
#include <tuple>
#include <cmath>
#include <limits>
#include <random>

#include <sciplot/sciplot.hpp>
using namespace std;
using namespace sciplot;

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
struct vertex {
    double cost, dist, heur;
    Point p;
};
struct comp {
    bool operator()(const vertex &a, const vertex &b ) {

        if (a.cost == b.cost) {
            //cout << a.cost << "\n";
            //if (a.heur == b.heur)
            return  a.heur > b.heur ;
        } else {
            //cout << fixed << setprecision(12) << "[" << a.cost << " , " << b.cost << "]\n";

            return a.cost > b.cost;
        }
        //return  a.cost == b.cost ? a.heur > b.heur : a.cost > b.cost;
        //return  a.heur == b.heur ? a.cost > b.cost : a.heur > b.heur;
    }
};

bool operator == (const Point &a, const Point &b) { return a.x == b.x && a.y == b.y; }
bool operator != (const Point &a, const Point &b) { return !(a == b); }
bool operator  < (const Point &a, const Point &b) { return make_pair (a.x,a.y) < make_pair (b.x,b.y); }

double distance (Point a, Point b) { return std::hypot (a.x - b.x, a.y - b.y); }
bool inside_circle (const Circle &c, const Point &p) { return distance (c.ctr, p) < c.r; }
int nearest_point (const vector<Point> &curr, const Point &a) {

    double minv = numeric_limits<double>::infinity(), dist;
    int near;

    for (int i = 0; i < curr.size(); i++) {
        dist = distance (a, curr[i]);

        if (a != curr[i] && dist < minv) {
            near = i;
            minv = dist;
        }
    }

    return near;
}

class Display {
    private :

        inline static Plot2D draw;
        static double maxp (const Point &p) { return max (abs (p.x), abs (p.y)); }

    public :
        static void point (const Point &p) {
            cout << "<" << setw(2) << p.x << "," << setw(2) << p.y << "> ";

        }

        static void img () {

            draw.legend().hide();

            Figure fig = {{draw}};

            Canvas canvas = {{fig}};
            canvas.size(800, 800);
            canvas.show();
        }

        static void dots (const vector<Point> &graph) {
            vector<double> x, y;
            for (auto &p : graph) {
                x.push_back (p.x);
                y.push_back (p.y);
            }

            draw.drawPoints(x, y).pointType(0);
        }
        static void line (const vector<Point> &graph) {
            vector<double> x, y;
            for (auto &p : graph) {
                x.push_back (p.x);
                y.push_back (p.y);
            }
            draw.drawBrokenCurveWithPoints(x, y);
        }
        static void graph (Point start, Point exit, vector<Circle> graph) {
          double  maxv = max (maxp (start), maxp (exit));

          for (auto &[ctr, rad] : graph) {
              maxv = max (maxp(ctr) + rad, maxv);
          }
          draw.xrange(-maxv, maxv);
          draw.yrange(-maxv, maxv);

          vector<double> ptx {start.x, exit.x}, pty {start.y,exit.y};
          draw.drawPoints(ptx,pty).pointType(4);

          for (auto [c,rad] : graph) {
              vector<double> x,y;

              for (double theta = 0.0; theta < 6.26; theta += 0.1) {
                  double ox = c.x + rad * sin (theta), oy = c.y + rad * cos (theta);
                  x.push_back (ox), y.push_back (oy);
              }

              draw.drawCurveFilled(x, y).fillColor("yellow");
          }

        }
        static void voronoi (const vector<Point> &graph) {

            random_device rd;
            mt19937 gen(rd());
            uniform_real_distribution<> rnd (0, 10);

            int size = 200000;
            Point p (0,0);
            vector<vector<Point>> region (graph.size());

            while (size-->0) {
                p = {rnd(gen), rnd(gen)};
                int near = nearest_point (graph, p);
                region[near].push_back (p);
            }

            dots (graph);

            for (int i = 0; i < region.size(); i++) {
                dots (region[i]);
            }

        }
        static vector<Point> generate (int size) {

            random_device rd;
            mt19937 gen(rd());
            uniform_real_distribution<> rnd (0, 10);

            vector<Point> graph;

            while (size-->0) {
                graph.push_back ({rnd(gen), rnd(gen)});
            }
            return graph;
        }
};

void intersect_line (Point p1, vector<Circle> space) {

    vector<Point> vect;

    const double radi = 0.5;

    for (double theta = 0.0; theta < 6.30; theta += 0.01) { // theta += 0.1
        Point p2 = {p1.x + radi * sin (theta), p1.y + radi * cos (theta)};

        double a = p2.y - p1.y;
        double b = p1.x - p2.x;
        double c = a * p1.x + b * p1.y;

        auto [ctr,rad] = space[0];
        double dist = (abs(a * ctr.x + b * ctr.y + c)) / sqrt(a * a + b * b);

        if (rad >= dist) {


        } else {


        }
    }
}
bool isvalid (const Point &p, const vector<Circle> &space) {

    for (auto &cir : space) {
        if (inside_circle (cir, p) == true) {
            return false;
        }
    }
    return true;
}
double heuristic (const Point &a, const Point &b) {
    return std::hypot (a.x - b.x, a.y - b.y);
}
void astar (Point start, Point exit, vector<Circle> space) {

    priority_queue<vertex,vector<vertex>,comp> q1;
    map<Point,bool> visit;

    q1.push ({0,0, distance (start,exit), start});

    int cycles = 0;
    vector<Point> vect;

    while (!q1.empty()) {

        auto [cost, dist, heur,  curr] = q1.top();
        q1.pop();
        visit[curr] = true;

        vect.push_back(curr);
        cycles++;
        //cout << fixed << cost << ' ' << heur << "\n";
        if (cycles > 500) {

            break;
        }

        const double rad = 0.01;
        double alt = (dist + rad) * 0.98;

        vector<Point> direct {{rad,0},{0,rad},{-rad,0},{0,-rad} /* ,{rad,rad},{-rad,rad},{rad,-rad},{-rad,-rad} */ };

        for (auto dir : direct) {
          double nx = curr.x + dir.x, ny = curr.y + dir.y;
          /*
        for (double theta = 0.0; theta < 6.30; theta += 0.2) { // theta += 0.1
            double nx = curr.x + rad * sin (theta), ny = curr.y + rad * cos (theta);
        */
            Point nxp = {nx, ny};
            double heu = distance (nxp, exit);
            double fnc = alt + heu;

            //alt = fnc (start, exit) - distance (start, nxp);

            if (isvalid (nxp, space) && !visit[nxp]) {
                //cout << fixed << setprecision(12) << "[" << nxp.x << " , " << nxp.y << "]\n";
                vertex nxv {fnc, alt, heu, nxp};
                q1.push(nxv);
            }
        }
    }

    //cout << visit.size();
    Display::dots(vect);
    while (!q1.empty()) {
        //auto [dist, heur, curr] = q1.top();
        q1.pop();
        //cout << fixed << dist << ' ' << heur << "\n";
    }

    /*
    */
}
void geom (Point start, Point exit, vector<Circle> space) {

    priority_queue<vertex,vector<vertex>,comp> q1;
    q1.push ({0,0, distance (start,exit), start});

    int cycles = 0;
    vector<Point> vect;

    while (!q1.empty()) {

        auto [cost, dist, heur,  p1] = q1.top();
        q1.pop();

        cycles++;
        //cout << fixed << cost << ' ' << heur << "\n";
        if (cycles > 1) {

            break;
        }
        const double radius = 0.1;

        for (double theta = 0.0; theta < 6.30; theta += 0.01) { // theta += 0.1
            Point p2 = {p1.x + radius * sin (theta), p1.y + radius * cos (theta)};

            double a = p2.y - p1.y;
            double b = p1.x - p2.x;
            double c = a * p1.x + b * p1.y;

            auto [ctr,rad] = space[0];
            double dist = ((a * ctr.x + b * ctr.y + c)) / sqrt(a * a + b * b);

            if (rad >= abs(dist)) {
              
            } else {


              p2 = {p1.x + 2 * sin (theta), p1.y + 2 * cos (theta)};
              //cout << (a * ctr.x + b * ctr.y + c) / sqrt(a * a + b * b) << "\n";
              //vertex nxv {fnc, alt, heu, nxp};
              //q1.push(nxv);
              vect.push_back(p2);
            }
            /*
            */
        }

    }

    //cout << visit.size();
    Display::dots(vect);
    /*
    */
}
int main () {

    const double max_error = 1e-8;

    Point start = {-3, 1}, exit = {4.25, 0};
    vector<Circle> space = { {0.0, 0.0, 2.5},/* {1.5, 2.0, 0.5}, {3.5, 1.0, 1.0}, {3.5, -1.7, 1.2} */};

    Display::graph (start, exit, space);

    geom (start, exit, space);

    Display::img();


    /*
    */
    //cout << fixed << setprecision(12) << max_error;
}

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

/*
Point nearest_point (const vector<Point> &grph, Point &p) {

    Point near, a = p;
    double min = numeric_limits<float>::infinity(), dist;

    for (auto &b : grph) {
        dist = distance ( a, b);

        if (a != b && dist < min) {
            min = dist;
            near = b;
        }
    }
    return near;
}
*/
