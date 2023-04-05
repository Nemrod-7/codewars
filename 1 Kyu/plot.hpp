//#include "graph.hpp"
#include <sciplot/sciplot.hpp>

using namespace sciplot;


class Draw {
    private :

        inline static Plot2D draw;
        static double maxp (const Point &p) { return max (abs (p.x), abs (p.y)); }
        static int nearest_point (const vector<Point> &curr, const Point &a) {

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
        static void rect (const vector<Point> &graph) {
            line ({graph[0], graph[1]});
            line ({graph[0], graph[2]});
            line ({graph[1], graph[3]});
            line ({graph[2], graph[3]});
        }
        static void dots (const vector<Point> &graph) {
            vector<double> x, y;
            for (auto &p : graph) {
                x.push_back (p.x);
                y.push_back (p.y);
            }

            draw.drawPoints (x, y).pointType(5);
        }
        static void line (const vector<Point> &graph) {
            vector<double> x, y;
            for (auto &p : graph) {
                x.push_back (p.x);
                y.push_back (p.y);
            }
            draw.drawCurve (x, y);
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

              for (double deg = 0.0; deg < 360; deg ++) {
                  double theta = deg * M_PI / 180;
                  double ox = c.x + rad * cos (theta), oy = c.y + rad * sin (theta);
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
