#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>

void Test ();

using point = std::pair<double,double>;

// constexpr double pi = std::acos(-1);
// double distance (const point &a, const point &b) { return hypot (b.second - a.second, b.first - a.first); }
// double gety (double x, pair<double,double> eq) { return eq.first * x + eq.second; }
// double getx (double y, pair<double,double> eq) { return (y - eq.second) / eq.first; }

double rnd (const double x) { return round (x * 10000) / 10000.0; }
double radian (const double deg) { return deg * M_PI / 180; }
double quadrant (double x) {

  if (x > 1) {
      int i1 = x;
      if (i1 % 2 == 0) {
        x = x - i1;
      } else {
        x = i1 * 2 - x;
      }
    }
    return rnd (x);
}
std::pair<double,double> laser_coord (double h, double a, double l) {

  if (a == 0.0)
    return {h,h};

  double dist = 0.0;
  double theta = radian (a);
  double x1 = 0.0, y1 = h;
  const double x2 = x1 + cos (theta), y2 = y1 + sin (theta);
  const double alt = hypot (y2 - y1, x2 - x1);

  double x3 = x1 + l / alt * (x2 - x1);
  double y3 = y1 + l / alt * (y2 - y1);

  return {quadrant (x3), quadrant (y3)};
}

int main () {

    double angle = 60.0, h = 0.5, length = 1.8;

    double dist = 0;
    point p1 = {0.0, h}, p2 = {0.2887, 1};

    /*

    1st bounce happens at (0.2887, 1) with total length 0.5773
    2nd bounce happens at (0.8660, 0) with total length 1.7320
    3rd bounce doesn't happen because the ray runs hits the length before hitting the wall, at (0.9,0.0588)

    y = m * x + ycept;
    x = (y - ycept) / m;
    */

    auto [x,y] = laser_coord (0.5,60,2.5);
    // std::cout << x << " " << y <<  "\n";
     Test();
    std::cout << "end";
}


static bool float_compare (double a, double b) {
  return std::fabs(a - b) < 1e-3;
}

void Test () {

    using test_t = std::tuple<double, double, double, std::pair<double, double>>;

    std::vector<test_t> tests {
      test_t{0.5, 60, 1.8, { 0.9, 0.0588}},
      test_t{0.5, 60, 2.5, {0.75, 0.6650}},
      test_t{  0,  0,   2, {   0,      0}},
      test_t{0.5,  0, 2.5, { 0.5,    0.5}},
      test_t{  0, 30,  10, {0.66,      1}},
      test_t{0.5,  0, 1.5, { 0.5,    0.5}},
    };

    for (const auto &[h, a, l, coord] : tests) {
      const auto p1 = laser_coord (h, a, l);
      const auto p2 = coord;

      if (p1 != p2) {
          std::cout << "\nexpected " << p2.first << " " << p2.second;
          std::cout << "\ngot      " << p1.first << " " << p1.second;
          std::cout << "\n";
      }
    }
};

bool isinside (double x, double y) { return x >= 0 && x <= 1 && y >= 0 && y <= 1; }
std::pair<double,double> laser_coord2 (double h, double a, double l) {

  if (a == 0.0) {
    return {h,h};
  }

  double dist = 0.0;
  double theta = radian (a), sign = 1.0;
  double x1 = 0.0, y1 = h;

  while (dist < l) {
      double x2 = x1 + cos (theta) * sign, y2 = y1 + sin (theta) * sign;
      double x, y, x3, y3;

      double m = (y2 - y1) / (x2 - x1), yc = y1 - (m * x1);
      // printf ("%6.3f %6.3f\n", theta, m);
      for (double i = 0; i < 2; i++) {
          x = (i - yc) / m, y = m * x + yc;

          if (isinside (x,y) && x != x1) {
              x3 = x, y3 = y;
          }

          y = m * i + yc, x = (y - yc) / m;

          if (isinside (x,y) && x != x1) {
              x3 = x, y3 = y;
          }
      }
      double alt = hypot (y3 - y1, x3 - x1);

      if (dist + alt > l) {
          double t = (l - dist) / alt;
          double x4 = x1 + t * (x3 - x1);
          double y4 = y1 + t * (y3 - y1);
          return {rnd (x4), rnd (y4)};
      }

      if (y3 == 1.0 || y3 == 0.0) {
          theta = -theta;
      }
      if (x3 == 1.0 || x3 == 0.0) {
          sign = -sign;
          theta = -theta;
      }

      dist += alt;
      x1 = x3, y1 = y3;
  }

  return {0.0, 0.0};
}
