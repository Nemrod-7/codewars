#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct point { double x, y; };

point operator + (const point &a, const point &b) { return {a.x + b.x, a.y + b.y}; }
double distance (const point &p, const point &q) { return std::hypot (p.x - q.x, p.y - q.y);}
double radian (double deg) { return deg * M_PI / 180.0; }

void pythagorasTree (point a, point b, int cycle){

	point e;
  const double width = b.x - a.x, height = b.y - a.y;
  //cout << a.x - b.x - height << ' '<< height + width << '\n';
  point c = {a.x - height, a.y + width};
  point d = {b.x - height, b.y + width};;
	e.x = d.x - (height + width) * 0.5;
	e.y = d.y - (height - width) * 0.5;

  //cout << distance (a,b) << " " << distance (a,c) << "\n";
	if (cycle > 0) {
		pythagorasTree (e, d, cycle - 1);
		pythagorasTree (c, e, cycle - 1);
	}
}
int main () {

    int t = 0;
    // triangle ratio : 3-4-5 -> base : 1 left 0.8, right = 0.6
    point start = {-3, 1}, exit = {4.25, 0};

    vector<point> vect;
    /*
    for (int i = 0; i < path.size(); i++) {
        point curr = path[i];
        if (collision (start, curr, space) == true) {
            start = curr;
            reduce.push_back (curr);
        }
    }
    */


    pythagorasTree (a,b,2);
    /*
    cout << tr.x << " " << tr.y;

    cout << '{';
    for (auto p : reduce) {
        cout << "{"<< p.x << "," << p.y <<  "},";
    }
    cout << "};";

    */

    cout << "\nend";

}
