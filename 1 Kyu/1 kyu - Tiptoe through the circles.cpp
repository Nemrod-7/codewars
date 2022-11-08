
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
		vector<Point> hist;
};
struct comp {
    bool operator()(const vertex &a, const vertex &b ) {
        return  a.cost == b.cost ? a.heur > b.heur : a.cost > b.cost;
        //return  a.heur == b.heur ? a.cost > b.cost : a.heur > b.heur;
    }
};

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

double sq (double x) { return x * x; }
double quadratic (double a, double b, double c) {

    double delta = b * b - 4 * a * c;
    double x1 = -(b + sqrt (delta)) / (2 * a);
    double x2 = c / (a * x1);

    return x2;
}
vector<Point> tangent (Point p1, Circle c) {
    auto [c1,rad] = c;
    double theta = acos (rad / distance (p1,c1));
    double direc = atan2 (p1.y - c1.y, p1.x - c1.x); // direction angle of point P from C

    const double d1 = direc + theta, d2 = direc - theta;
    const Point t1 = {c1.x + rad * cos(d1), c1.y + rad * sin(d1)}; // tangent to circle c1 => line : p1 ~ p2
    const Point t2 = {c1.x + rad * cos(d2), c1.y + rad * sin(d2)}; // tangent to circle c1 => line : p1 ~ p2
    return {t1,t2};
}
vector<Point> interception (Point p1, Point p2, Circle c) {
    auto [c2,r2] = c;
    const Point p3 = {p1.x - c2.x, p1.y - c2.y}, p4 = {p2.x - c2.x, p2.y - c2.y}; //shifted line points

    const double m = (p4.y - p3.y) / (p4.x - p3.x); // slope of the line
    const double b = p3.y - m * p3.x;               // y intercept of line
    const double np = sq(r2) * sq(m) + sq (r2) - sq(b); // quad equation

    if (np > 0) {
        const double t1 = (-m * b + sqrt(np)) / (sq(m) + 1);
        const double t2 = (-m * b - sqrt(np)) / (sq(m) + 1);

        const Point i1 = {t1 + c2.x, m * t1 + b + c2.y};
        const Point i2 = {t2 + c2.x, m * t2 + b + c2.y};
        return {i1,i2};
    }

    return {};
}
vector<Point> find_tangent (Point p1, const vector<Circle> &space) {

		const double epsilon = 1e-8;
    const vector<double> sign {1 , -1 };

		vector<Point> vect;

    for (Circle c1 : space) {
        for (Point &p2 : tangent (p1, c1)) {
            const double dist = distance (p1, p2);

            bool valid = true;
            for (Circle c2 : space) {

                if (c2.ctr != c1.ctr) {
                    vector<Point> interc = interception (p1, p2, c2);
                    for (auto &i1 : interc) {
                        if (distance (p1, i1) + distance (i1, p2) - dist <= epsilon) {
                            valid = false;
                        }
                    }
                }
            }

            if (valid == true) {
                vect.push_back(p2);
            }
        }
    }

		return vect;
}

void astar (Point start, Point exit, vector<Circle> space) {
		const double rad = 0.1;

		priority_queue<vertex,vector<vertex>,comp> q1;
		map<Point,bool> visit;
		Graph system (start, exit, space);

		Draw::graph(start, exit, space);
		vertex source = {0,0,distance (start,exit),{start}};
		q1.push (source);

		int cycles = 0;
		vector<Point> vect;

		while (!q1.empty()) {

				auto [cost, dist, left, path] = q1.top();
				q1.pop();

				Point curr = path.back();
				cycles++;

				if (cycles > 1500 || distance (curr,exit) < 0.3) {
						vect = path;
						break;
				}

				//rad = minrad (curr, star); // 0.1
				double alt = (dist + rad) * 1.0;

				vector<Point> direct { {rad,0},{0,rad},{-rad,0},{0,-rad},  {rad,rad},{-rad,rad},{rad,-rad},{-rad,-rad} };

				for (auto &dir : direct) {
						double nx = curr.x + dir.x, ny = curr.y + dir.y;
						Point nxp = {nx, ny};

						//if (space.isinside (nxp)) {
						double heu = distance (nxp, exit);
						double fnc = alt + heu;

						if (isvalid (nxp, space) && !visit[nxp]) {
								path.push_back(nxp);
								vertex nxv {fnc, alt, heu, path};
								visit[nxp] = true;
								q1.push(nxv);
						}
				//}
				}
		}

		Draw::dots(vect);
		Draw::img();
}
bool isdirect (Point p, Point dest, vector<Circle> &space) {

		bool direct = true;

		for (Circle &star : space) {
				if (interception (p, dest, star).size() != 0) {
						return false;	
				}
		}
		return true;
}
int main () {

    Point start = {-3, 1}, exit = {4.25, 0};
    vector<Circle> space = {  {0.0, 0.0, 2.5}, {1.5, 2.0, 0.5}, {3.5, 1.0, 1.0}, {3.5, -1.7, 1.2} };

    //Draw::graph (start,exit,space);
    
    const double epsilon = 1e-8;
    const vector<double> sign {1 , -1 };
    const int size = space.size();

		Graph univ (start,exit,space);
		vector<Point> vect;

		//vect = interception (start,exit,space);
		astar(start, exit, space);
 
		/*
    Draw::dots(vect);
    Draw::img();
    */
}
////////////////////////////Arkive////////////////////////////////

