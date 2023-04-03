#include <iostream>
#include <fstream>
#include <sstream>

#include <cmath>
#include <vector>

using namespace std;
using point = pair<double,double>;

void showpoint (const pair<double,double> &p) {
    cout << "[" << p.first << "," << p.second << "]\n";
}

vector<double> tokenize (const string &src, char delim) {
    istringstream iss (src);
    string token;
    vector<double> v;

    while (getline (iss, token, delim))
        v.push_back (stoi(token));

    return v;
}

double rnd (double x) { return round (x * 1e5) / 1e5; }
double dist (const point &p, const point &q) { return std::hypot (p.first - q.first, p.second - q.second);}

pair<double,double> barycentre (const point &p1, const point &p2, const point &p3) {
    double x = (p1.first + p2.first + p3.first) / 3;
    double y = (p1.second + p2.second + p3.second) / 3;
    return {x, y};
}

bool is_right (const point &p1, const point &p2, const point &p3) {
    double a = dist (p1, p2), b = dist (p2, p3), c = dist (p1, p3);
    double a2 = rnd (a * a), b2 = rnd (b * b), c2 = rnd (c * c);
    return (a2 + b2 == c2 || b2 + c2 == a2 || a2 + c2 == b2);
}
double area (const point &a, const point &b, const point &c) {
    double x1 = a.first, x2 = b.first, x3 = c.first;
    double y1 = a.second, y2 = b.second, y3 = c.second;
    return rnd (abs (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2);
}
bool is_inside (const point &p, const point &a, const point &b, const point &c) { // check if p lies inside triangle A B C
    const double epsilon = 1e-06;
    double aa = area (a, b, p), bb = area (a, p, c), cc = area (p, b, c);
    double total = area (a,b,c), trigl = aa + bb + cc;
    if (abs (total - trigl) <= epsilon) return true;
    return false;
}

vector<point> mkpoint (int size) {
  vector<point> ve;
  for (int x = 0; x < size; x++) {
      for (int y = 0 ; y < size; y++) {
          if (x != 0 || y != 0) {
              ve.push_back({x,y});
              //cout << x << ' ' << y << '\n';
          }
      }
  }
  return ve;
}

int main () {


    const double epsilon = 1e-06, limit = 2.0;
    const point center = {0.0,0.0};

    int cnt = 0;
    vector<point> ve = mkpoint (51);
    int n  = ve.size();

    for (int i = 0; i < ve.size(); i++) {
        //auto p1 = ve[i];
        for (int j = i + 1; j < ve.size(); j++) {
            //auto p2 = ve[j];
          if (is_right (center, ve[i], ve[j])) {
              cnt++;
          }
        }
    }

    int cnt2 = n * (n - 1) / 4;

    cout << "res :: " << cnt;

}
