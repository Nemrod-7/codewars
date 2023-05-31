#include <iostream>
#include <vector>
#include <cmath>


using namespace std;

struct point { double x, y; };

point operator + (const point &a, const point &b) { return {a.x + b.x, a.y + b.y}; }
double distance (const point &p, const point &q) { return hypot (p.x - q.x, p.y - q.y);}
double radian (double deg) { return deg * M_PI / 180.0; }
int64_t gcd (int64_t a, int64_t b) { return b == 0 ? a : gcd (b, a % b); }

bool issquare (uint64_t num) {
		if (num < 2) return false;
    uint64_t sq = sqrt(num);
    return sq * sq == num;
}
void pythagorasTree (point a, point b, int cycle) {

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


void showvec (vector<int64_t> seq) {
	cout << "[ ";
	for (auto it : seq) {
			cout << it << " ";
	}
	cout << "]\n";
}

int main () {


    cout << "\nend";

}
