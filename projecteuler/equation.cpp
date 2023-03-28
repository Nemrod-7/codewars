#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <map>
#include <chrono>

using namespace std;

typedef unsigned __int128 uint128_t;

double trunc (double x) { return round (x * 1e12) / 1e12; }
bool is_square (double x) {
    int sq = sqrt(x);
    return (sq * sq) == (int) x;
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

vector<int> convergence (const double &x) {
    if (is_square (x)) {

      return {};}

    const int sq = sqrt(x);
    int an = 2 * sq, y = sq, z = 1;
    vector<int> frac {sq};

    do {
      y = an * z - y;
      z = (x - y * y) / z;
      an = (sq + y) / z;
      frac.push_back(an);
    } while (an != 2 * sq);

    return frac;
}
uint64_t diophantine5 (int delta) {
    double sq = sqrt (delta);
    if (is_square (delta)) return 1;

    vector<int> a = convergence (sq);
    vector<uint64_t> p = {0,1, uint64_t(a[0])}, q = {1,0,1};

    for (int k = 0; k < 20; k++) {
        uint64_t an = a[k % (a.size() - 1) + 1];
        uint64_t x = an * p[k+2] + p[k+1];
        uint64_t y = an * q[k+2] + q[k+1];
        int64_t eq = x * x - (uint64_t) delta * y * y;
        //cout << eq << ' ';
        cout << x  * x << "^2 - " << delta << " * " << y << "^2 => " << eq ;
        cout << endl;
        if (eq == 1) {
            return x;
        }

        p.push_back(x), q.push_back(y);
    }

    return 0;
}

void showvec (vector<int> ve) {
  cout << "[" << ve[0] << ";";
  for (int i = 1; i < ve.size(); i++) {
    cout << ' ' << ve[i];
  }
  cout << "]\n";
}

uint64_t pell (int64_t delta) { // pell's equation
    const int64_t sq = sqrt (delta);
    if (sq * sq == delta) return 1;

    int64_t an = 2 * sq, y = sq, z = 1;
    uint64_t x, b;
    vector<int64_t> p = {1,0}, q = {0,1};

    while (true) {
        //cout << an << ' ' << z << ' ' << y << '\n';
        y = an * z - y;
        z = (delta - y * y) / z;
        an = (sq + y) / z;

        p = {p[1], an * p[1] + p[0]};
        q = {q[1], an * q[1] + q[0]};

        x = p[1] + sq * q[1];
        b = q[1];

        if (x * x - delta * b * b == 1) {
            cout << x << "^2 - " << delta << " * " << b << "^2 => ";
            //cout << (b + 1) / 2 << " " << (x + 1) / 2;
            cout << '\n';
            break;
        }
    }

    return x;
}

int main () {

    auto start = std::chrono::high_resolution_clock::now();
    // 1766319049^2 - 61 * 226153980^2 => 1
    //  29718^2 - 61 * 3805^2 == -1
    const double eps = 1e-8;
    int cnt = 0;
    auto frac = convergence (2);
    int dec = frac.size() - 1;
    //pell(2);
    cout << "\n";

    /*
    for (int i = 0; i < 8; i++) {
        int64_t b = v[i] * (v[i] - 1);

        for (int64_t blu = v[i] / 2; blu < v[i]; blu++) {
            int64_t a = 2 * (blu * (blu - 1));

            if (b - a == 0) {
              cout << blu << " " << v[i] << " " << 0 << "\n";
              break;
            }

        }
    }
    */

     auto end = std::chrono::high_resolution_clock::now();
     std::chrono::duration<double> elapsed = end - start;
     std::cout << "\nProcess took " << elapsed.count()  << " ms" << std::endl;
}
////////////////////////////////////////////////////////////////////////////////

vector<int> convergence2 (const double x) {

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

        if (cycle[trunc (fp)]) break;
        cycle[trunc (fp)] = true;

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
    for (int k = 0; k < size; k++) {
        cout << p[k] << ' ';
    }
    return 0;
}
int64_t diophantine4 (int delta) {
  const double sq = sqrt (delta);
  if (is_square (delta)) return 1;

  double rn = sq, ip = floor(rn);
  vector<uint64_t> p = {0,1}, q = {1,0};

  for (int k = 2; k < 25; k++) {
      cout << trunc (ip) << ' ';
      uint64_t x  = round(ip) * p[k-1] + p[k-2];
      uint64_t y = round(ip) * q[k-1] + q[k-2];
      uint64_t x2 = x * x;
      uint64_t y2 = y * y;
      uint64_t z = delta * y2;
      int64_t eq = x2 - z;

      //cout << x << "^2 - " << delta << " * " << y << "^2 == " << eq << "\n";

      if (x * x - delta * y * y == 1) {
          return x;
      }
      rn = 1 / (rn - ip);
      ip = floor (rn);

      p.push_back(x), q.push_back(y);
  }
  return 0;
}
