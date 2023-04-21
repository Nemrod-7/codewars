#include <iostream>
#include "graph.hpp"
#include "plot.hpp"
// using namespace std;
const double ratio = 3.0 / 4.0;
const double alpha = atan (ratio);

double degree (double rad) { return rad * 180.0 / M_PI; }

Point barycentre (const Point &p1, const Point &p2, const Point &p3) {
  double x = (p1.x + p2.x + p3.x) / 3;
  double y = (p1.y + p2.y + p3.y) / 3;
  return {x, y};
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

  const double rad = 0.5;
  // double m = slope (p1,p2);
  // double cosine = 1.0 / sqrt (1.0 + sq (m)), sine = m / sqrt (1.0 + sq (m));

  for (double deg = 0; deg < 90; deg++) {
      double alpha = radian(deg);
  //    Point p2 = {p1.x + rad * cos (alpha), p1.y + rad * sin (alpha)};
  //    node.push_back(p2);
  }

}

void pythagorasTree (Point a, Point b, int cycle) {

  const double width = b.x - a.x, height = b.y - a.y;
  const Point c = {a.x - height, a.y + width}, d = {b.x - height, b.y + width};
  // cout << width << " " << d.x - c.x << "\n";
  const double hyp = hypot (width, height);
  const double adj = hyp * 4.00 / 5.0, opp = hyp * 3.0 / 5.0;
  // printf ("%f ", atan(opp /adj));
  //     theta = angle of triangle + base angle
  const double theta = alpha + atan2 (height, width);
  const Point i1 = {c.x + adj * cos(theta), c.y + adj * sin(theta)};
  // Draw::rect({a,b,c,d});

  if (cycle > 0) {
      pythagorasTree (i1, d, cycle - 1);
      pythagorasTree (c, i1, cycle - 1);
  }
}

int main () {


    /*

    trigonometry spreadsheet => theta is angle between hypothenus and adjacent size
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

    cout << fixed << setprecision(3);

    vector<Point> border {{-5,-5},{-5,5},{5,5},{5,-5},{-5,-5}};

    cout << 180.0 / 360.0;
    // Draw::img();


    cout << "\nend\n";
}


void pythagorasTree2 (Point a, Point b, int cycle) {

	Point e;
  const double width = b.x - a.x, height = b.y - a.y;
  //cout << a.x - b.x - height << ' '<< height + width << '\n';
  Point c = {a.x - height, a.y + width};
  Point d = {b.x - height, b.y + width};

	e.x = d.x - (height + width) * 0.5;
	e.y = d.y - (height - width) * 0.5;

  // Draw::rect ({a,b,c,d});

  if (cycle > 0) {
		pythagorasTree2 (e, d, cycle - 1);
		pythagorasTree2 (c, e, cycle - 1);
	}
}
