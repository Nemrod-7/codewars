#include <cmath>

using namespace std;

double areaOfPolygonInsideCircle (double rad, int n ) {
  const double PI =  3.14159265 ;
  double ar = pow (rad, 2) * n * sin (2 * PI / n) / 2;
  return round (ar * 1e3) / 1e3;
}
