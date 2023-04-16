
#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <tuple>
#include <cmath>
#include <limits>

#include "graph.hpp"
#include "plot.hpp"

/*
trigonometry spreadsheet

theta = asin (opp / hyp);
theta = acos (adj / hyp);
theta = atan (opp / adj);

*/

using namespace std;

const double epsilon = 1e-8;

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

double astar_distance_proto (Point p1, Point p2, vector<Circle> graph) {
    double alt;
    int id1 = identify (p1, graph);
    double ab = distance (p1,p2);
    int id2 = identify (p2, graph);

    if (id2 == id1) { // if p1 and p2 lies on the same sphere : dist = arc length
        auto [p3,rad] = graph[id2];
        const double hyp = distance (p1,p3);
        alt = 2 * rad * asin (0.5 * ab / hyp);
    } else {
        alt = ab;
    }
    return alt;
}

double area (const vector<Point> &curr) {
    double sum = 0;

    for (size_t i = 0; i < curr.size(); ++i) {
        sum += (curr[i].x * curr[i].y + 1) - (curr[i].x + 1 * curr[i].y);
    }

    return abs (sum) / 2;
}
double ccw (const Point &o, const Point &a, const Point &b) { // counter clockwise
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}
vector<Point> convex_hull (vector<Point> curr) {

    const int size = curr.size();
    int i, t, k = 0;
    vector<Point> hull (size * 2);
    //sort (begin(curr), end(curr), vertcmp);

    for (i = 0; i < size; i++) {
        while (k >= 2 && ccw (hull[k - 2], hull[k - 1], curr[i]) <= 0) {
            k--;
        }

        hull[k++] = curr[i];
    }

    for (i = size - 2, t = k + 1; i >= 0; i--) {
        while (k >= t && ccw (hull[k - 2], hull[k - 1], curr[i]) <= 0) {
            k--;
        }
        hull[k++] = curr[i];
    }

  //return vector<point>(h.begin(), h.begin() + k - (k > 1));
    hull.resize (k - 1);
    return hull;
}

void astar2 (Point start, Point exit, vector<Circle> graph) {

  struct vertex { double dist; vector<Point> path; };
  struct comp {
      bool operator() (const vertex &a, const vertex &b ) {
          return  a.dist > b.dist;
      }
  };

  const int size = graph.size();
  vector<vector<int>> neigh (size);
  vector<vector<Point>> edge;
  map<Point,int> base;

  priority_queue<vertex,vector<vertex>,comp> q1;
  map<Point,bool> visit;

  edge = tangraph (graph);
  // cout << fixed << setprecision (5);
  for (int i = 0; i < edge.size(); i++) { // attach edges to circles
      for (int j = 0; j < edge[i].size(); j++) {
          int id = identify (edge[i][j], graph);
          neigh[id].push_back(i);
      }
      // int id1 = identify (edge[i][0], graph), id2 = identify (edge[i][1], graph);
      // neigh[id1].push_back(i); neigh[id2].push_back(i);
      // Display::vect(edge[i]);
  }

  for (auto p : find_tangent (exit,graph)) { // connect exit tangents to tan visibility graph
      int id = identify (p, graph);
      edge.push_back({p, exit}); // -> exit return -1 !!
      neigh[id].push_back(edge.size());
  }

  for (auto p : find_tangent (start,graph)) {
      vertex source = {distance (start,p),{start,p}};
      q1.push(source);
  }

  int cycle = 0;

  while (!q1.empty()) {
      auto [heur,path] = q1.top();
      Point p1 = path.back();
      q1.pop();

      cycle++;

      if (cycle == 2) {
        // Draw::line(path);
          Display::vect(path);
          cout << "\n";
          break;
      }

      // nearsest point on a circle
      int id = identify (p1, graph);
      vector<Point> nxp;
      double minv = numeric_limits<double>::infinity();

      for (int i = 0; i < neigh[id].size(); i++) {
          vector<Point> nxe = edge[neigh[id][i]];
          int ida = identify (nxe[0], graph);

          if (ida != id) swap (nxe[0], nxe[1]);

          Point tmp = nxe[0];
          double dist = distance (p1,tmp);
          //cout << ida << " " << idb << "\n";
          if (dist < minv) {
              minv = dist;
              nxp = nxe;
          }
      }
      auto [p3,rad] = graph[id];
      const double ab = distance (p1,nxp[0]);
      const double hyp = distance (p1,p3);
      //            arc legth                      + distance to next circle
      double alt = 2 * rad * asin (0.5 * ab / hyp) + distance (nxp[0], nxp[1]);

      vector<Point> route = path;

      for (auto e : nxp) {
        route.push_back(e);
      }

      vertex nextv = {heur + alt, route};
      q1.push(nextv);
  }

    /*
    for (int i = 0; i < neigh.size(); i++) {
        cout << "circle " << i << " :: ";
        for (int j = 0; j < neigh[i].size(); j++) {
            cout << neigh[i][j] << " ";
        }
        cout << "\n";
    }
  */
}

int main () {

    Point start = {-3, 1}, exit = {4.25, 0};
    vector<Circle> graph = {{0.0, 0.0, 2.5}, {1.5, 2.0, 0.5}, {3.5, 1.0, 1.0}, {3.5, -1.7, 1.2}};
    // Draw::graph (start,exit,graph);
    astar2(start,exit,graph);
    // Draw::img();

}
////////////////////////////Arkive////////////////////////////////
void mkcircle (Point p1, Point p2) {

  double m = slope (p1,p2);
  const double rad = 0.5;
  double cosine = 1.0 / sqrt (1.0 + sq (m)), sine = m / sqrt (1.0 + sq (m));

  for (double deg = 0; deg < 90; deg++) {
      double alpha = radian(deg);
  //    Point p2 = {p1.x + rad * cos (alpha), p1.y + rad * sin (alpha)};
  //    node.push_back(p2);
  }

}
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
double degree (double radian) { return radian * 180.0 / M_PI; }

void arclenth (Point a, Point b, Point c, double r) { // arc length of circle segment a-b
    double ab = distance (a,b) / 2, hyp = distance (a,c); // ab -> opposite side, hyp = hypthenuse
    double theta = degree (asin (ab / hyp));
    double arclen = 2 * M_PI * r * (theta / 360);
}
void astar (Point start, Point exit, vector<Circle> graph) {
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
