#include <iostream>
#include "graph.hpp"
#include "plot.hpp"
// using namespace std;

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

double degree (double radian) { return radian * 180.0 / M_PI; }

void pythagorasTree (Point a, Point b, int cycle) {

	Point e;
  const double width = b.x - a.x, height = b.y - a.y;
  //cout << a.x - b.x - height << ' '<< height + width << '\n';
  Point c = {a.x - height, a.y + width};
  Point d = {b.x - height, b.y + width};

	e.x = d.x - (height + width) * 0.5;
	e.y = d.y - (height - width) * 0.5;

  // Draw::rect ({a,b,c,d});

  if (cycle > 0) {
		pythagorasTree (e, d, cycle - 1);
		pythagorasTree (c, e, cycle - 1);
	}
}
double distfromline (Point a, Point b, Point c) { // return distance of point C from line AB
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

void mktriangle () {
  vector<Point> vect;
  Point a = {-2.0, 2.0}, b = {4.0, -3.0}, c;

  // a = {0,0}, b = {3,0};
  Point mid = {(a.x + b.x) * 0.5, (a.y + b.y) * 0.5};

  double hyp = hypot (a.x - b.x, a.y - b.y);
  double adj = hyp * 4.00 / 5.0, opp = hyp * 3.0 / 5.0;

  double rad = hyp / 2;

  double theta = asin (rad / opp); // atan (3.0 / 4.0)
  //c = {mid.x + hyp * cos (theta) * cos (theta), mid.y + hyp * cos (theta) * sin (theta)};

  cout << setprecision (4);//
  vect.push_back (mid);
  cout << "\n";

  for (double deg = 0; deg < 80; deg += 1) {
     theta = radian (deg);
     Point p2 = {mid.x + rad * cos (theta), mid.y + rad * sin (theta)};
     vect.push_back(p2);
     //cout << distance (a,p2) << " " << distance (b,p2) << " => " << theta << " :: " << deg << "\n";
  }

  // cout << adj << " " << opp << " => " << theta << "\n";
  cout << adj << " " << distance (a,c) << "\n";
  cout << opp << " " << distance (b,c) << "\n";

  double m = -1 / slope (a,b) ;
  double y = a.y - (m * a.x);


}


int main () {


    /*
    trigonometry spreadsheet => theta is angle between hypothenus and adjacent size
    Soh cah toa

    theta = asin (opp / hyp);
    theta = acos (adj / hyp);
    theta = atan (opp / adj);
    */

    cout << fixed << setprecision(3);

    double angle = 60.0, h = 0.5, length = 1.8;

    double theta = radian (angle), sign = 1.0;
    Point p1 = {0.0,h};
    double step = 0.01;
    vector<Point> vect = {p1};

    Draw::line({{0,0},{0,1},{1,1},{1,0},{0,0}});

    for (double i = 0; i < 5.0; i += step) {
        double nx = p1.x + step * cos (theta) * sign, ny = p1.y + step * sin (theta) * sign;
        // cout << nx << " " << ny << "\n";
        if (ny > 1.0 || ny < 0.0) {
          theta = -theta;
        }
        if (nx > 1.0 || nx < 0.0) {
          sign = -sign;
          theta = -theta;
        }
        p1 = {p1.x + step * cos (theta) * sign, p1.y + step * sin (theta) * sign};

        vect.push_back(p1);
    }
    /*
    */

    Draw::dots(vect);
    Draw::img();

    /*

    pythagorasTree ({0.0,0.0}, {5.0,0.0}, 4);

    */

    cout << "\nend\n";
}
