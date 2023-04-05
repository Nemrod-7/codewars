
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

const double epsilon = 1e-8;

struct vertex {
    double cost, dist, heur;
		vector<Point> hist;
};
struct comp {
    bool operator()(const vertex &a, const vertex &b ) {
        return  a.cost == b.cost ? a.heur > b.heur : a.cost > b.cost;
        //return  a.heur == b.heur ? a.cost > b.cost : a.heur > b.heur;
    }
};


double sq (const double x) { return x * x; }
double radian (const double deg) { return deg * M_PI / 180.0; }
double slope (const Point &a, const Point &b) { return (b.y - a.y) / (b.x - a.x); }
double intercept (const Point &a, const double m) { return a.y - (m * a.x); }
double quadratic (const double a, const double b, const double c) {

    const double delta = b * b - 4 * a * c;
    const double x1 = -(b + sqrt (delta)) / (2 * a);
    const double x2 = c / (a * x1);

    return x2;
}

vector<Point> tangent (const Point &p1, const Circle &c) { // tangent points of a cricle
    auto [c1,rad] = c;
    const double theta = acos (rad / distance (p1,c1));
    const double direc = atan2 (p1.y - c1.y, p1.x - c1.x); // direction angle of point P from C

    const double d1 = direc + theta, d2 = direc - theta;
    const Point t1 = {c1.x + rad * cos (d1), c1.y + rad * sin (d1)}; // tangent to circle c1 => line : p1 ~ p2
    const Point t2 = {c1.x + rad * cos (d2), c1.y + rad * sin (d2)}; // tangent to circle c1 => line : p1 ~ p2
    return {t1,t2};
}
vector<Point> interception (const Point &p1, const Point &p2, const Circle &c) { // interception points of line and circle
    auto [c2,r2] = c;
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
vector<Point> hcenter (const Circle &c1, const Circle &c2) { // homothetic centers
    Point h1, h2; // : 1 => external, 2 => internal
    const auto [p1,r1] = c1;
    const auto [p2,r2] = c2;
    const double mn = r1 - r2, mx = r1 + r2;

    h1.x = -r2 / mn * p1.x + r1 / mn * p2.x;
    h1.y = -r2 / mn * p1.y + r1 / mn * p2.y;

    h2.x = r2 / mx * p1.x + r1 / mx * p2.x;
    h2.y = r2 / mx * p1.y + r1 / mx * p2.y;
    return {h1,h2};
}

bool collision (Point &p1, Point &p2, vector<Circle> &graph) {
    const double dist = distance (p1,p2);

		for (Circle &c1 : graph) {
        if (inside_circle (p1, c1) || inside_circle (p2, c1)) {
           return true;
        }
        for (auto ip : interception (p1, p2, c1)) {
            if (distance (p1, ip) + distance (ip, p2) <= dist) {
                return true;
            }
        }
		}

    return false;
}

void astar (Point start, Point exit, vector<Circle> graph) {
		const double rad = 0.1;
    const vector<Point> direct { {rad,0.0},{0.0,rad},{-rad,0.0},{0.0,-rad},  {rad,rad},{-rad,rad},{rad,-rad},{-rad,-rad} };

		priority_queue<vertex,vector<vertex>,comp> q1;
		map<Point,bool> visit;
		//Graph system (start, exit, graph);
		//Draw::graph(start, exit, graph);
		vertex source = {0,0,distance (start,exit),{start}};
		q1.push (source);

		int cycles = 0;
		vector<Point> vect;

		while (!q1.empty()) {

				auto [cost, dist, left, path] = q1.top();
				q1.pop();

				Point curr = path.back();
				cycles++;
        //vect.push_back(curr);
				if (cycles > 1500 || distance (curr,exit) < 0.3) {
						vect = path;
						break;
				}

        double alt = (dist + rad) * 1.0;

				for (auto &dir : direct) {
						double nx = rnd (curr.x + dir.x), ny = rnd (curr.y + dir.y);
						Point nxp = {nx, ny};
            //cout << nx << " " << ny << "\n";
						//if (graph.isinside (nxp)) {
						double heu = distance (nxp, exit);
						double fnc = alt + heu;

						if (is_valid (nxp, graph) && !visit[nxp]) {
								path.push_back(nxp);
								vertex nxv {fnc, alt, heu, path};
								visit[nxp] = true;
								q1.push(nxv);
						}
				//}
				}
		}

    cout << '{';
    for (auto p : vect) {
        cout << "{"<< p.x << "," << p.y <<  "},";
    }
    cout << "};";
 		//Draw::dots(vect);
		//Draw::img();
}
vector<vector<Point>> mknode (vector<Circle> graph) {

  const int size = graph.size();
  vector<vector<Point>> edge;

  for (int i = 0; i < size; i++) {
    Circle c1 = graph[i];

    for (int j = i + 1; j < size; j++) {
      Circle c2 = graph[j];
      //vector<Point> hc = hcenter (c1, c2);
      for (auto p2 : hcenter (c1, c2)) {
        // Point p2 = hc[k];
        if (is_valid (p2, graph)) {
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

int main () {

    Point start = {-3, 1}, exit = {4.25, 0};
    vector<Circle> graph = {  {0.0, 0.0, 2.5}, {1.5, 2.0, 0.5}, {3.5, 1.0, 1.0}, {3.5, -1.7, 1.2} };

    const int size = graph.size();
    auto edge = mknode (graph);

    // cout << fixed << setprecision (5);
    vector<Point> vect;
    Point p1 = exit;

    for (int i = 0; i < size; i++) {
        Circle c1 = graph[i];
        for (Point p2 : tangent (p1, c1)) {
            const double dist = distance (p1, p2);
            bool valid = true;
            // Display::point (p2);
            /*
            */
            for (int j = i + 1; j < size; j++) {
                Circle c2 = graph[j];
                vector<Point> interc = interception (p1, p2, c2);

                for (auto &i1 : interc) {
                    //vect.push_back(i1);
                    // cout << distance (p1, i1) + distance (i1, p2) << " " << dist << "\n";
                    if (distance (p1, i1) + distance (i1, p2) <= dist) {
                        valid = false;
                    }
                }
            }

            if (valid == true) {
            //     vect.push_back(p2);
                Draw::line({exit,p2});
            }
        }
    }

    /*
    for (auto e : edge) {
        Draw::line(e);
    }


    double m = slope (p1,p2);
    const double rad = 0.5;
    double cosine = 1.0 / sqrt (1.0 + sq (m)), sine = m / sqrt (1.0 + sq (m));

    for (double deg = 0; deg < 90; deg++) {
        double alpha = radian(deg);
    //    Point p2 = {p1.x + rad * cos (alpha), p1.y + rad * sin (alpha)};
    //    edge.push_back(p2);
    }
		// astar(start, exit, graph);

    Draw::graph (start,exit,graph);
    //Draw::dots (vect);
    Draw::img();

    */
}
////////////////////////////Arkive////////////////////////////////

pair<double,double> barycentre (const Point &p1, const Point &p2, const Point &p3) {
  double x = (p1.x + p2.x + p3.x) / 3;
  double y = (p1.y + p2.y + p3.y) / 3;
  return {x, y};
}
double area (const Point &a, const Point &b, const Point &c) { // triangle area
    double x1 = a.x, x2 = b.x, x3 = c.x;
    double y1 = a.y, y2 = b.y, y3 = c.y;
    return rnd (abs (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) * 0.5);
    //area2 = abs((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y))
}

class Graph {
    private :

    public :
				double minx, maxx, miny, maxy;

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

                if (inside_circle(start, ci) || inside_circle(exit, ci)) {

                }
                /*
                */
            }
            maxx += pad, maxy += pad, minx -= pad, miny -= pad;
            //cout << minx << " " << maxx << " " << miny << " " << maxy;
        }
        bool isinside (const Point &p) { return p.x > minx && p.x < maxx && p.y > miny && p.y < maxy; }
};

vector<Point> find_tangent (const Point &p1, const vector<Circle> &space) {

		const double epsilon = 1e-8;
    //const vector<double> sign {1.0,-1.0};
		vector<Point> vect;

    for (Circle c1 : space) {
        for (Point p2 : tangent (p1, c1)) {
            const double dist = distance (p1, p2);
            bool valid = true;
            // Display::point (p2);
            for (Circle c2 : space) {
                if (c2.ctr != c1.ctr) {
                    vector<Point> interc = interception (p1, p2, c2);

                    for (auto &i1 : interc) {

                        if (distance (p1, i1) + distance (i1, p2) - dist <= epsilon) {
                            valid = false;
                        }
                        /*
                        */
                    }
                }
            }

            if (valid == true) {
                vect.push_back(p2);
            }
        }
    }
    // Draw::dots (vect);
		return vect;
}
