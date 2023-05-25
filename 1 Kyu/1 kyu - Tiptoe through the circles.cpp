
#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <cmath>
#include <limits>

#include "graph.hpp"
#include "plot.hpp"

/*

trigonometry  => theta is angle between hypothenus and adjacent size
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

using namespace std;

struct vertex { double dist; vector<Point> path; };
struct comp {
    bool operator() (const vertex &a, const vertex &b ) {
        return  a.dist > b.dist;
    }
};

class Base {
    public :
        vector<vector<int>> edge;
        vector<vector<Point>> gate, inter;
        map<Point,int> hist;

        Base (int size) {
            edge.resize(size);
            inter.resize(size);
        }
};

const double epsilon = 1e-8;

double clamp (double x, double mini, double maxi) { // limit a position to an area
  if (x < mini) x = mini;
  if (x > maxi) x = maxi;
  return x;
}
double degree (double radian) { return radian * 180.0 / M_PI; }

vector<Point> tangent (const Point &p1, const Circle &c) { // tangent points of a cricle
    const auto &[c1,rad] = c;
    const double theta = acos (rad / distance (p1,c1));
    const double direc = atan2 (p1.y - c1.y, p1.x - c1.x); // direction angle of point P from C

    const double d1 = direc + theta, d2 = direc - theta;
    const Point t1 = {c1.x + rad * cos (d1), c1.y + rad * sin (d1)}; // tangent to circle c1 => line : p1 ~ p2
    const Point t2 = {c1.x + rad * cos (d2), c1.y + rad * sin (d2)}; // tangent to circle c1 => line : p1 ~ p2
    return {t1,t2};
}
vector<Point> interception (const Point &p1, const Point &p2, const Circle &c) { // interception points of line and circle
    const auto &[c2,r2] = c;
    const Point p3 = {p1.x - c2.x, p1.y - c2.y}, p4 = {p2.x - c2.x, p2.y - c2.y}; //shifted line points

    const double m = (p4.y - p3.y) / (p4.x - p3.x);    // slope of the line
    const double b = p3.y - m * p3.x;                  // y intercept of line
    const double np = sq(r2) * sq(m) + sq(r2) - sq(b); // quad equation

    if (np > 1e-4) {
        const double t1 = (-m * b + sqrt(np)) / (sq(m) + 1);
        const double t2 = (-m * b - sqrt(np)) / (sq(m) + 1);

        const Point i1 = {t1 + c2.x, m * t1 + b + c2.y};
        const Point i2 = {t2 + c2.x, m * t2 + b + c2.y};
        return {i1,i2};
    }

    return {};
}
vector<Point> circles_intersection (const Circle &c1, const Circle &c2) { // intersection points of 2 circles
    const auto &[p1,r1] = c1;
    const auto &[p2,r2] = c2;
    const double dist = distance (p1, p2);

    if (dist <= r1 + r2 && dist >= fabs (r2 - r1)) {
        const double ex = (p2.x - p1.x) / dist;
        const double ey = (p2.y - p1.y) / dist;

        const double a = (sq(r1) - sq(r2) + sq(dist)) / (2 * dist);
        const double h = sqrt (r1 * r1 - a * a);
        // Point tmp = {(p1.x + (a * (c2.ctr.x - p1.x)) / dist), (p1.y + a * (c2.ctr.y - p1.y) / dist)};
        const Point p3 = {p1.x + a * ex - h * ey, p1.y + a * ey + h * ex};
        const Point p4 = {p1.x + a * ex + h * ey, p1.y + a * ey - h * ex};
        return {p3,p4};
    }
    return {};
}
vector<Point> hcenter (const Circle &c1, const Circle &c2) { // homothetic centers
    const auto &[p1,r1] = c1;
    const auto &[p2,r2] = c2;
    const double mn = r1 - r2, mx = r1 + r2;
    Point h1, h2; // : 1 => external, 2 => internal

    h1.x = -r2 / mn * p1.x + r1 / mn * p2.x;
    h1.y = -r2 / mn * p1.y + r1 / mn * p2.y;

    h2.x = r2 / mx * p1.x + r1 / mx * p2.x;
    h2.y = r2 / mx * p1.y + r1 / mx * p2.y;
    return {h1,h2};
}

bool collision (const Point &p1, const Point &p2, const vector<Circle> &graph) {
    const double dist = distance (p1,p2);

		for (const Circle &c1 : graph) {
        if (inside_circle (p1, c1) || inside_circle (p2, c1)) {
           return true;
        }
        for (auto ip : interception (p1, p2, c1)) {
            if ((distance (p1, ip) + distance (ip, p2) - dist) <= epsilon) {
                return true;
            }
        }
		}

    return false;
}

vector<vector<Point>> tangraph (vector<Circle> graph) {  // tangent visibility graph

  const int size = graph.size();
  vector<vector<Point>> edge;

  for (int i = 0; i < size; i++) {
    Circle c1 = graph[i];

    for (int j = i + 1; j < size; j++) {
      Circle c2 = graph[j];

      for (auto p2 : hcenter (c1, c2)) {
        if (is_free (p2, graph)) {
          vector<Point> t1 = tangent (p2, c1), t2 = tangent (p2, c2);

          if (collision (t1[0], t2[0], graph) == false) {
              edge.push_back ({t1[0],t2[0]});
          }

          if (collision (t1[1], t2[1], graph) == false) {
              edge.push_back ({t1[1],t2[1]});
          }
        }
      }
    }
  }

  return edge;
}
vector<Point> find_tangent (const Point &p1, const vector<Circle> &graph) { // find valid tangent from point
    const int size = graph.size();
		vector<Point> vect;

    for (int i = 0; i < size; i++) {
        Circle c1 = graph[i];
        for (Point p2 : tangent (p1, c1)) {
            if (collision (p1,p2, graph) == false) {
                vect.push_back(p2);
            }
        }
    }

		return vect;
}
int identify (const Point &p1, const vector<Circle> &graph) {

    for (int i = 0; i < graph.size(); i++) {
        auto [p2,rad] = graph[i];
        //cout << distance (graph[i].ctr, p) << " " << graph[i].r << "\n";
        if (abs (distance (p1, p2) - rad) < epsilon) {
            return i;
        }
    }
    return -1;
}

Base mkgraph (const Point &start, const Point &exit, const vector<Circle> &graph) {
    const int size = graph.size();
    Base base (size);
    vector<vector<Point>> gate = tangraph (graph);

    for (int i = 0; i < gate.size(); i++) { // reference edges to circles
        for (int j = 0; j < gate[i].size(); j++) {
            Point p = gate[i][j];
            int id = identify (p, graph);

            base.hist[p] = id;
            base.edge[id].push_back(i);
        }
    }

    for (auto p : find_tangent (exit,graph)) { // connect exit tangents to tan visibility graph
        int id = identify (p, graph);
        gate.push_back({p, exit}); // -> exit return -1 !!
        base.hist[p] = id;
        base.edge[id].push_back(gate.size());
    }

    for (int i = 0; i < size; i++) { //         // check for circle intersections
        Circle c1 = graph[i];
        for (int j = i + 1; j < size; j++) { // check for circle intersections
            vector<Point> cip = circles_intersection (c1, graph[j]);
            for (auto p : cip) {
                base.inter[i].push_back(p);
                base.inter[j].push_back(p);
            }
        }
    }

    base.gate = gate;
    return base;
}

void astar3 (Point start, Point exit, vector<Circle> graph) {

    const int size = graph.size();
    auto [edge,gate,inter,base] = mkgraph (start,exit,graph);
    priority_queue<vertex,vector<vertex>,comp> q1;

    for (auto p : find_tangent (start,graph)) {
        q1.push (vertex {distance (start,p), {start,p}});
        // printf ("{%f %f}", p.x, p.y);
    }

    int cycle = 0;

    while (!q1.empty()) {
        auto [dist,path] = q1.top();
        Point p1 = path.back();
        q1.pop();

        cycle++;
        if (cycle == 2) {
            break;
        }

        if (p1 == exit) {
            break;
        }

        int id = base[p1];      // get circle id
        Circle c1 = graph[id];  // get circle

        double minv = numeric_limits<double>::max();

        for (auto p2 : inter[id]) {                                      // search nearest intersection
            double theta = asin (distance (p1,p2) * 0.5 / c1.r) * 2.0; // in radian
            minv = min (minv, theta);
        }

        for (int i = 0; i < edge[id].size(); i++) {
            vector<Point> nxe = gate[edge[id][i]];
            if (base[nxe[0]] != id)
                swap (nxe[0],nxe[1]);
            // printf ("{%f %f}", nxe[0].x, nxe[0].y);
            Point p2 = nxe[0];
            double theta = asin (distance (p1,p2) * 0.5 / c1.r) * 2.0; // in radian

            if (theta < minv) {
                double arc = c1.r * theta;
                double alt = arc + distance (p2,nxe[1]);
                // 2 * M_PI * c1.r * theta / 360.0;  // double arc =  c1.r * 2.0 * asin (distance (p1,p2) * 0.5 / c1.r); // arc length
                vector<Point> route = path;
                for (auto p : nxe) {
                    route.push_back(p);
                }

                q1.push({alt,route});
            }
        }
    }
    while (!q1.empty()) {
        auto [dist,hist] = q1.top();
        q1.pop();
        Display::point(hist.back());
    }
}

int main () {

    // cout << fixed << setprecision(3);

    Point start = {-3, 1}, exit = {4.25, 0};
    vector<Circle> graph = {{0.0, 0.0, 2.5}, {1.5, 2.0, 0.5}, {3.5, 1.0, 1.0}, {3.5, -1.7, 1.2}};
    const int size = graph.size();

    vector<Point> vect;
    vector<vector<Point>> edge;

    int id = 0;
    auto [c1,r1] = graph[id];

    for (int i = 0; i < size; i++) {
        if (i != id) {
            auto [c2,r2] = graph[i];
            double dist = distance (c1,c2);

            if (r1 + r2 > dist) {

            }
        }
    }
    /*
    const double hyp = r1, opp = distance (a,b) * 0.5;

    const double theta = asin (opp/hyp) * 2.0;
    const double arc = r1 * theta;

    const double inv = r1 * (2 * M_PI - theta);
    */

    astar3 (start,exit,graph);

    /*
    Draw::graph (start,exit,graph);
    // Draw::dots(vect);
    Draw::img();


    // double u = ​(B − A) * (B − A)​ / ​(C − A) * (B − A)​/​
    // double u = ((p3.x - p1.x) * (p2.x - p1.x) + (p3.y - p1.y) * (p2.y - p1.y)) / distance (p1, p2);
    // double E = A + clamp (u, 0.0, 1.0) * (B - A);
    // E.x = p1.x + clamp (u, 0, 1) * (p2.x - p1.x);
    // E.y = p1.y + clamp (u, 0, 1) * (p2.y - p1.y);

    */

}
