#include <iostream>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

struct point { double x, y; };

point operator + (const point &a, const point &b) { return {a.x + b.x, a.y + b.y}; }
double distance (const point &p, const point &q) { return std::hypot (p.x - q.x, p.y - q.y);}
double radian (double deg) { return deg * M_PI / 180.0; }
int64_t gcd (int64_t a, int64_t b) { return b == 0 ? a : gcd (b, a % b); }

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

int pythagoreantriplet (int sum) {
    const int s2 = sum / 2;
    const int lim = ceil(sqrt(s2)) - 1;
    int cnt = 0;

    for (int m = 2; m <= lim; m++) {
        if (s2 % m == 0) {
            int sm = s2 / m;
            int k = (m % 2 == 1) ? m + 2 : m + 1;

            while (sm % 2 == 0)
                sm /= 2;

            while ((k < (2 * m)) && (k <= sm)) {
                if (sm % k == 0 && gcd (k,m) == 1) {
                    //cout << k << ' ';
                    int d = s2 / (k * m);
                    int n = k - m;

                    int a = d * (m * m - n * n);
                    int b = 2 * d * m * n;
                    int c = d * (m * m + n * n);

                    if (a + b + c == sum) {
                      //printf ("[%i, %i %i] ", a,b,c);
                        cnt++;
                    }
                }
                k += 2;
            }
        }
    }

    return cnt;
}

int main () {

    int t = 0;
    // triangle ratio : 3-4-5 -> base : 1 left 0.8, right = 0.6
		int lim = 120000;
		// int base = 272, hyp = 305, h = 273;

		map<int64_t,bool> sq;

		for (int64_t i = 1; i < lim; i++) {
				sq[i * i] = true;
		}

		vector<double> sig = {-1.0,1.0};
		// bc2 = ab2 + ac2
		for (int64_t ac = 2; ac < lim; ac++) {
				int64_t base = ac * 2;

				for (auto sg : sig) {
						int64_t ab = base + sg;
						int64_t bc2 = ac * ac + ab * ab;

						if (sq[bc2] == true) {
							cout << "base : " << base << " hyp : " << sqrt (bc2) << " h: " << ab << "\n";
						}

				}
			// cout << "\n";
		}


    /*

		point start = {-3, 1}, exit = {4.25, 0};
		vector<point> vect;

    for (int i = 0; i < path.size(); i++) {
        point curr = path[i];
        if (collision (start, curr, space) == true) {
            start = curr;
            reduce.push_back (curr);
        }
    }
		pythagorasTree (a,b,2);

		*/

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
