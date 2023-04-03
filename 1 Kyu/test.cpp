#include <iostream>
#include "graph.hpp"
#include "plot.hpp"
// using namespace std;

double sq (const double x) { return x * x; }
vector<Point> interception (const Point &p1, const Point &p2, Circle &c) {
    auto [c2,r2] = c;
    const Point p3 = {p1.x - c2.x, p1.y - c2.y}, p4 = {p2.x - c2.x, p2.y - c2.y}; //shifted line Points

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
bool collision (Point &p, Point &dest, vector<Circle> &space) {

		bool direct = true;

		for (Circle &star : space) {
				if (interception (p, dest, star).size() != 0) {
						return false;
				}
		}
		return true;
}
vector<Point> reduce (vector<Point> &path, vector<Circle> &space) {
    vector<Point> reduce {path[0]};

    return reduce;
}

double radian (double deg) { return deg * M_PI / 180.0; }

void pythagorasTree (Point a, Point b, int cycle) {

	Point e;
  const double width = b.x - a.x, height = b.y - a.y;
  //cout << a.x - b.x - height << ' '<< height + width << '\n';
  Point c = {a.x - height, a.y + width};
  Point d = {b.x - height, b.y + width};;

	e.x = d.x - (height + width) * 0.5;
	e.y = d.y - (height - width) * 0.5;

  Draw::rect ({a,b,c,d});

  if (cycle > 0) {
		pythagorasTree (e, d, cycle - 1);
		pythagorasTree (c, e, cycle - 1);
	}
}
double dstfromline (Point a, Point b, Point c) {
    double hyp = distance (a,c), line = distance (a,b);
    if (line == 0.0) return hyp;
    /*
    double da = a.y - b.y;
    double db = b.x - a.x;
    double dc = a.x * b.y - b.x * a.y;
    // |da * c.x + db * c.y + dc| / sqrt (da^2 + db^2);
    */
    return abs ((b.x - a.x) * (a.y - c.y) - (a.x - c.x) * (b.y - a.y)) / line;
}

int main () {

    Point start = {-3, 1}, exit = {4.25, 0};
    vector<Circle> space = {  {0.0, 0.0, 2.5}, {1.5, 2.0, 0.5}, {3.5, 1.0, 1.0}, {3.5, -1.7, 1.2} };

    vector<Point> path {{-3,1},{-2.9,1},{-3,1.1},{-3.1,1},{-3,0.9},{-2.9,1.1},{-3.1,1.1},{-2.9,0.9},{-2.8,0.9},{-2.9,0.8},{-2.8,1},{-3,1},{-2.8,0.8},{-2.7,0.8},{-2.8,0.7},{-2.7,0.9},{-2.6,1},{-2.5,1.1},{-2.4,1.2},{-2.3,1.3},{-2.2,1.4},{-2.1,1.5},{-2,1.6},{-1.9,1.7},{-1.8,1.8},{-1.7,1.9},{-1.6,2},{-1.5,2},{-1.4,2.1},{-1.3,2.2},{-1.2,2.2},{-1.1,2.3},{-1,2.3},{-0.9,2.4},{-0.8,2.4},{-0.7,2.4},{-0.6,2.5},{-0.5,2.5},{-0.4,2.5},{-0.3,2.5},{-0.2,2.5},{-0.1,2.5},{0,2.5},{0.1,2.5},{0.2,2.5},{0.3,2.5},{0.4,2.5},{0.5,2.5},{0.6,2.5},{0.7,2.5},{0.7,2.6},{0.7,2.4},{0.8,2.4},{0.9,2.4},{1,2.4},{1,2.5},{1,2.3},{1.1,2.4},{1.2,2.5},{1.3,2.5},{1.4,2.5},{1.5,2.5},{1.6,2.5},{1.7,2.5},{1.8,2.5},{1.9,2.5},{1.9,2.6},{1.9,2.4},{2,2.4},{2,2.5},{2,2.3},{2.1,2.3},{2,2.2},{2.1,2.4},{2.1,2.2},{2.2,2.2},{2.1,2.1},{2.2,2.3},{2.2,2.1},{2.3,2.1},{2.2,2},{2.3,2.2},{2.3,2},{2.4,2},{2.3,1.9},{2.4,2.1},{2.4,1.9},{2.5,1.9},{2.4,1.8},{2.5,2},{2.5,1.8},{2.6,1.8},{2.5,1.7},{2.6,1.9},{2.6,1.7},{2.7,1.7},{2.6,1.6},{2.6,1.5},{2.5,1.4},{2.5,1.3},{2.5,1.2},{2.5,1.1},{2.5,1},{2.5,0.9},{2.5,0.8},{2.5,0.7},{2.5,0.6},{2.5,0.5},{2.6,0.5},{2.6,0.4},{2.6,0.3},{2.7,0.3},{2.7,0.2},{2.8,0.2},{2.9,0.2},{2.8,0.1},{2.9,0.1},{3,0.1},{2.9,0},{3,0},{3.1,0},{3.2,0},{3.3,0},{3.4,0},{3.5,0},{3.6,0},{3.7,0},{3.8,0},{3.9,0},{4,0}};

    //cout << path.size() << "\n";
    vector<Point> vect;

    Point a = {-2.0, 2.0}, b = {4.0, -3.0}, c;
    Point center = {(a.x + b.x) * 0.5, (a.y + b.y) * 0.5};

    double base = std::hypot (a.x - b.x, a.y - b.y);
    double left = base * 4.00 / 5.0, right = base * 3.0 / 5.0;

    const double rad = base / 2;
    double minv = 999.0, minagl;
    Point tr;
    //return abs ((b.x - a.x) * (a.y - c.y) - (a.x - c.x) * (b.y - a.y)) / base;

    for (double deg = 0; deg < 360; deg += 0.1) {
        double angle = radian (deg);
        Point p2 = {center.x + rad * cos (angle), center.y + rad * sin (angle)};
        double t = distance (a, p2) - left;

        if (t > 0) {
            if (t < minv) {
                minv = t;
                tr = p2;
            }
        }
    }

    vect.push_back(tr);
    /*
    double alpha = π / 2 * w(v2) / (w(v1) + w(v2)) ; // w : weight of node

    const double m = slope (a,b) ;
    const double y = a.y - (m * a.x);

    for (double i = 0; i < 3; i += 0.1) {
        const double nx = center.x + i;
        const double ny = m * nx + y;

        if (nx < b.x) {
            //cout << ny << ' ';
            vect.push_back({nx,ny});
        }
    }
    */

    Draw::dots({{-5,5},{5,5},{-5,-5},{5,5}});
    Draw::dots(vect);

    Draw::line({a,b});
    Draw::dots({center});
    Draw::img();

    /*

    pythagorasTree ({0.0,0.0}, {5.0,0.0}, 4);

    for (int i = 0; i < path.size(); i++) {
        Point curr = path[i];
        if (collision (start, curr, space) == true) {
            start = curr;
            vect.push_ccack (curr);
            //cout << i << " ";
        }
    }

    */

    cout << "\nend\n";
}
