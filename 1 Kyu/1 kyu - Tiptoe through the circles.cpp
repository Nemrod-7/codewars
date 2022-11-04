
#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <tuple>
#include <cmath>
#include <limits>

#include "graph.hpp"
#include "plot.hpp"

using namespace std;

struct vertex {
    double cost, dist, heur;
    Point p;
};
struct comp {
    bool operator()(const vertex &a, const vertex &b ) {
        return  a.cost == b.cost ? a.heur > b.heur : a.cost > b.cost;
        //return  a.heur == b.heur ? a.cost > b.cost : a.heur > b.heur;
    }
};

double sq (double x) { return x * x; }
void intersect_line (Point p1, vector<Circle> space) {

    vector<Point> vect;

    const double radi = 0.8;

    for (double theta = 0.0; theta < 6.30; theta += 0.01) { // theta += 0.1
        Point p2 = {p1.x + radi * sin (theta), p1.y + radi * cos (theta)};

        double a = p2.y - p1.y;
        double b = p1.x - p2.x;
        double m = a / b;
        double c = a * p1.x + b * p1.y;
        cout << p2.y - m * p2.x << " " << c << "\n";
        auto [ctr,rad] = space[0];

        double dist = (abs(a * ctr.x + b * ctr.y + c)) / sqrt(a * a + b * b);

        if (dist < rad) {
 // compute distance from t to circle intersection point
            double dt = sqrt (rad * rad - dist * dist);
              if (radi > dt) {
                vect.push_back (p2);
            }
        } else if (dist == rad) {


        } else {
            vect.push_back(p2);

        }
    }
    Draw::dots(vect);
}
double minrad (const Point &p1, const vector<Circle> &space) {

    double rad = numeric_limits<double>::infinity();

    for (auto &star : space) {
        double dist = distance (p1, star.ctr) - star.r;
        rad = min (rad,dist);
    }

    return  max(rad, 0.1);
}

class Graph {
    private :
        double minx, maxx, miny, maxy;

    public :
        Graph (Point start, Point exit, vector<Circle> space) {

            const double pad = 0.2;
            minx = min (start.x, exit.x), maxx = max (start.x, exit.x);
            miny = min (start.y, exit.y), maxy = max (start.y, exit.y);

            for (auto ci : space) {
                Point p = ci.ctr;

                minx = min (p.x - ci.r, minx);
                maxx = max (p.x + ci.r, maxx);
                miny = min (p.y - ci.r, miny);
                maxy = max (p.y + ci.r, maxy);

                if (inside_circle(ci, start) || inside_circle(ci, exit)) {

                }
                /*
                */
            }
            maxx += pad, maxy += pad, minx -= pad, miny -= pad;
            //cout << minx << " " << maxx << " " << miny << " " << maxy;
        }
        bool isinside (const Point &p) { return p.x > minx && p.x < maxx && p.y > miny && p.y < maxy; }
};

void astar (Point start, Point exit, vector<Circle> star) {

    priority_queue<vertex,vector<vertex>,comp> q1;
    map<Point,bool> visit;
    Graph space (start, exit, star);

    //Draw::graph(start,exit,star);

    q1.push ({0,0, distance (start,exit), start});

    int cycles = 0;
    vector<Point> vect;

     while (!q1.empty()) {

          auto [cost, dist, left,  curr] = q1.top();
          q1.pop();
          //Display::point (curr);
          cycles++;
          vect.push_back(curr);

          if (cycles > 1500 || distance (curr,exit) < 0.3) {

              break;
          }

          double rad = 0.1;
          //rad = minrad (curr, star); // 0.1
          double alt = (dist + rad) * 1.0;

          vector<Point> direct { {rad,0},{0,rad},{-rad,0},{0,-rad},  {rad,rad},{-rad,rad},{rad,-rad},{-rad,-rad} };

          for (auto &dir : direct) {
              double nx = curr.x + dir.x, ny = curr.y + dir.y;
              Point nxp = {nx, ny};

              //if (space.isinside (nxp)) {
                  double heu = distance (nxp, exit);
                  double fnc = alt + heu;

                  if (isvalid (nxp, star) && !visit[nxp]) {

                      vertex nxv {fnc, alt, heu, nxp};
                      visit[nxp] = true;
                      q1.push(nxv);
                  }
              //}
          }
      }

    //Draw::dots(vect);
    //Draw::img();
}
pair<Point,Point> tangent (Point p1, Circle c) {

    auto &[p2,rad] = c;

    double dist = distance (p1,p2);
    double dir = atan2 (p1.y - p2.y, p1.x - p2.x); // # direction angle of point P from C
    double theta = acos (rad / dist);

    double d1 = dir + theta; // # direction angle of point T1 from C
    double d2 = dir - theta; // # direction angle of point T2 from C

    double ax = p2.x + rad, ay = p2.y + rad;

    Point t1 {ax * cos(d1), ay * sin(d1)};
    Point t2 {ax * cos(d2), ay * sin(d2)};

    return {t1,t2};
}

bool intersect2 (Point a, Point b, Circle c) {


    double dist = distance (a,b);
    // compute the direction vector D from A to B
    double dx = (b.x - a.x) / dist;
    double dy = (b.y - b.y) / dist;

    // the equation of the line AB is x = dx*t + a.x, y = dy*t + b.y with 0 <= t <= dist.
    // compute the distance between the points A and E, where
    // E is the poi:nt of AB closest the circle center (c.x, c.y)
    double t = dx * (c.ctr.x - a.x) + dy * (c.ctr.y - b.y);

    Point E = {t * dx + a.x, t * dy + b.y}; // compute the coordinates of the point E

    double np = distance (E, c.ctr); // compute the euclidean distance between E and C

    // test if the line intersects the circle
    if (np < c.r) {
        /*
        // compute distance from t to circle intersection point
        double dt = sqrt (c.r * c.r - np * np);

        // compute first intersection point
        Fx = (t - dt) * dx + a.x;
        Fy = (t - dt) * dy + b.y;

        // compute second intersection point
        Gx = (t + dt) * dx + a.x;
        Gy = (t + dt) * dy + b.y;
         */

    } else if (np == c.r) { // else test if the line is tangent to circle

      // tangent point to circle is E
    } else {
      // line doesn't touch circle

    }

    return false;
}
void proto_isvalid (Point start, Point exit, vector<Circle> space ) {

    vector<Point> vect;
    Point p1 = start;
    vector<double> sign {1,-1};

    for (auto [p2,rad] : space) {

        double theta = acos (rad / distance (p1,p2));
        double direc = atan2 (p1.y - p2.y, p1.x - p2.x); // direction angle of point P from C

        for (auto sig : sign) {
            double nd = direc + theta * sig;
            Point t = {p2.x + rad * cos(nd), p2.y + rad * sin(nd)};

            bool valid = true;

            Point a = p1, b = t;

            for (auto c : space) {

            }
        }
    }

}

int main () {

    const double max_error = 1e-8;

    Point exit = {-3, 1}, start = {4.25, 0};
    vector<Circle> space = { {0.0, 0.0, 2.5}, {1.5, 2.0, 0.5}, {3.5, 1.0, 1.0}, {3.5, -1.7, 1.2} };

    Draw::graph (start,exit,space);
    vector<double> sign {1,-1};
    vector<Point> vect;

    Point p1 = exit;

    auto [c1,rad] = space[0];

    double theta = acos (rad / distance (p1,c1));
    double direc = atan2 (p1.y - c1.y, p1.x - c1.x); // direction angle of point P from C

    for (auto sig : sign) {
        double nd = direc + theta * sig;
        Point p2 = {c1.x + rad * cos(nd), c1.y + rad * sin(nd)}; // tangent to circle c1
        // line = p1 ~ p2
        bool valid = true;

        for (auto [c2, r2] : space) {
            Point p3 = {p1.x - c2.x, p1.y - c2.y}; //shifted line points
            Point p4 = {p2.x - c2.x, p2.y - c2.y};
                                                                                                   
            double m = (p4.y - p3.y) / (p4.x - p3.x); // slope of the line
            double b = p3.y - m * p3.x;               // y-intercept of line
            double under =  sq(r2) * sq(m) + sq (r2) - sq(b);

            if (under < 0) {
                //line completely missed
                //vect.push_back(p2);
                //return false;
            } else if (under > 0) {
                double t1 = (-m * b + sqrt(under)) / (sq(m) + 1); //one of the intercept x's
                double t2 = (-m * b - sqrt(under)) / (sq(m) + 1); //other intercept's x

                Point i1 = {t1 + c2.x, m * t1 + b + c2.y}; //intercept point 1
                Point i2 = {t2 + c2.x, m * t2 + b + c2.y}; //intercept point 2
                //cout << p4.x << " " << p3.x << endl;
                Display::point(i1);
                Display::point(i2);
                //vect.push_back(i1);
                //vect.push_back(i2);
            }
        }
    }

    Draw::dots(vect);
    Draw::img();
    /*
    */

    //astar(start,exit,star);
}
