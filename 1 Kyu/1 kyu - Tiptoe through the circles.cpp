
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
int main () {

    const double max_error = 1e-8;


    Point start = {-3, 1}, exit = {4.25, 0};
    vector<Circle> star = { {0.0, 0.0, 2.5}, {1.5, 2.0, 0.5}, {3.5, 1.0, 1.0}, {3.5, -1.7, 1.2} };
    
    //Draw::graph (start,exit,star);
    
    vector<Point> vect;
    Point p1 = start; 
    auto &[p2,rad] = star[0];

    double dist = distance (p1,p2);
    double theta = acos (rad / dist);  
    double dir = atan2 (p1.y - p2.y, p1.x - p2.x); // # direction angle of point P from C

    double d1 = dir + theta; // # direction angle of point T1 from C
    double d2 = dir - theta; // # direction angle of point T2 from C

    double ax = p2.x + rad, ay = p2.y + rad;
    
    Point t1 {ax * cos(d1), ay * sin(d1)};
    Point t2 {ax * cos(d2), ay * sin(d2)};

    vect.push_back(t1);
    vect.push_back(t2);
    
    Display::point(t1);
    Display::point(t2);

    Draw::dots(vect);
    Draw::img();      


    //astar(start,exit,star);
 }


