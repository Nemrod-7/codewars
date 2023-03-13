#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <chrono>

using namespace std;

double trunc (double x) { return round (x * 1e5) / 1e5; }
bool is_square (double x) {
    int sq = sqrt(x);
    return (sq * sq) == (int) x;
}
vector<int> convergence (const double x) {

    vector<int> frac;
    double rn = x, ip = floor(rn), fp = rn - ip;
    map<double, bool> cycle;
    const double thresh = 1e-8;
    //printf ("%f => ", rn);
    frac.push_back (static_cast<int> (round(ip)));

    while (fp > thresh) {
        rn = 1 / fp;
        ip = floor (rn);
        fp = rn - ip;

        if (cycle[trunc (fp)]) {
            break;
        } else {
            cycle[trunc (fp)] = true;
        }
        frac.push_back (static_cast<int> (round(ip)));
        //printf ("%5.2f %5.2f %5.2f\n", ip, fp, rn);
    }

    return frac;
}

int64_t diophantine1 (int64_t d) {

    for (int64_t x = 2; x < 100000; x++) {
        for (int64_t y = 1; y < x; y++) {
            if ((x * x) - d * (y * y) == 1) {
                cout << x << "^2 - " << d << " * " << y << "^2";
                return x;
            }
        }
    }

    return 0;
}
int64_t diophantine2 (int delta) {

    int64_t x = sqrt (delta), y = 1;
    int64_t eq = x * x - delta * y * y;
    int64_t cycle = 0;
    while (cycle < 1e12) {

        while (eq < 1) {
          x++;
          eq = x * x - delta * y * y;
        }
        while (eq > 1) {
          y++;
          eq = x * x - delta * y * y;
        }

        if (eq == 1) {
            cout << x << "^2 - " << delta << " * " << y << "^2 => " << eq ;
            //cout << " :: " << x / (double) y << " " << sqrt(delta);

            cout << "\n";
            return x;
        }

        cycle++;
    }

    return 0;
}
int64_t diophantine3 (int delta) {

    double sq = sqrt (delta);
    if (is_square (delta)) return 1;

    vector<int> a = convergence(sq);
    int size = a.size() + 2;
    int64_t p[size] = {0,1}, q[size] = {1,0};

    for (int k = 2; k < size; k++) {
        p[k] = a[k-2] * p[k-1] + p[k-2];
        q[k] = a[k-2] * q[k-1] + q[k-2];

        int64_t eq = p[k] * p[k] - delta * q[k] * q[k];
        //int64_t eq2 = q[k] * q[k] - delta * p[k] * p[k];
        if (eq == 1) {
            cout << p[k] << "^2 - " << delta << " * " << q[k] << "^2 => " << eq ;
            return p[k];
        }
        //cout << p[k] << ' ' << q[k] << " :: " << eq << '\n';
    }

    return 0;
}

void render (vector<int> ve) {
  cout << "[" << ve[0] << ";";
  for (int i = 1; i < ve.size(); i++) {
    cout << ' ' << ve[i];
  }
  cout << "]\n";
}

pair<int, int> getFraction (const vector<int> &a) {
  int size = a.size();
  int num = a[size-1], den = 1;

  for(int i = size - 2; i >= 0; i--) {
    int nextd = num * a[i] + den;
    den = num;
    num = nextd;
    cout << num << ' ' << den << '\n';
  }
  return make_pair (num, den);
}

int main () {

    auto start = std::chrono::high_resolution_clock::now();


    const double eps = 1e-8;
    int n, maxv = 0, res;

    ///diophantine3 (11);

    map<int,int> sqr;
    for (int i = 1; i < 100; i++) {
        sqr[i*i] = true;
    }


    for (int i = 1; i < 100; i++) {
        if (sqr[i] == false) {

            cout << i << " = > ";
            int x = diophantine3 (i);
            //render (convergence (sqrt (i)));
            cout  << "\n";
            if ( x > maxv ) {
                maxv = x;
                res = i;
            }

        }
    }

    /*
     * x^2 - z * y^2 == 1
     * sqrt (-z * y^2)  == 1 / (x^2 )
     *
     */

     auto end = std::chrono::high_resolution_clock::now();
     std::chrono::duration<double> elapsed = end - start;
     std::cout << "\nProcess took " << elapsed.count()  << " ms" << std::endl;
  }
