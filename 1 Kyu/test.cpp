#include <iostream>
#include "graph.hpp"
// #include "plot.hpp"
// using namespace std;
const double ratio = 3.0 / 4.0;
const double alpha = atan (ratio);

Point barycentre (const Point &p1, const Point &p2, const Point &p3) {
    double x = (p1.x + p2.x + p3.x) / 3;
    double y = (p1.y + p2.y + p3.y) / 3;
    return {x, y};
}
double distfromline (Point a, Point b, Point c) { // return distance of point C from line AB
    double hyp = magnitude (a,c), line = magnitude (a,b);
    if (line == 0.0) return hyp;
    /*
    double da = a.y - b.y;
    double db = b.x - a.x;
    double dc = a.x * b.y - b.x * a.y;
    // |da * c.x + db * c.y + dc| / sqrt (da^2 + db^2);
    */
    return abs ((b.x - a.x) * (a.y - c.y) - (a.x - c.x) * (b.y - a.y)) / line;
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
Point inversepoint (const Point &p1, const Point &p2) { // inverse point : p3 = p1 - p2
    return {p1.x - (p2.x - p1.x), p1.y - (p2.y - p1.y)};
}

int main () {

    // vector<Point> border {{-5,-5},{-5,5},{5,5},{5,-5},{-5,-5}};
    cout << fixed << setprecision(3);

    Circle c1 ({0.0, 0.0}, 1.5);
    auto [p1,r1] = c1;

    Point a (0.0 , 1.5), b = {1.5, 0.0}; // two points on the circle

    double perimeter = 2.0 * M_PI * r1;

    const double hyp = r1, opp = magnitude (a,b) * 0.5;

    const double theta = asin (opp / hyp) * 2.0;
    const double arc = r1 * theta;

    const double inv = r1 * (2 * M_PI - theta);


    cout << perimeter << " " << arc + inv;
    // double m = slope (p1,p2);
    // double cosine = 1.0 / sqrt (1.0 + sq (m)), sine = m / sqrt (1.0 + sq (m));
    // Draw::range(5,5);
    // Draw::circle (c1);
    //
    // Draw::dots({a,b});
    // Draw::dots({{x2,y2}});
    // // Draw::dots(ve);
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
