#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

double distance (const vector<double> &a, const vector<double> &b) {
    return hypot (a[0] - b[0], a[1] - b[1], a[2] - b[2]);
}

bool detectint (double x, double y, double z) {
    const double step = 0.02;
    const vector<double> origin = {0,0,0}, exit = {x,y,z};
    double mind = 9999999.9, dist;
    vector<double> p {0,0,0};

    for (double i = 0; i <= x; i += step) {
        p[0] = i, p[1] = y, p[2] = 0;
        dist = distance (origin, p) + distance (p, exit);

        if (floor(dist) == dist) {
            cout << dist;
            return  true;
        }
        mind = min (mind, dist);
    }

    for (double i = 0; i <= y; i += step) {
        p[0] = x, p[1] = i, p[2] = 0;
        dist = distance (origin, p) + distance (p, exit);

        if (floor(dist) == dist) {
            return  true;
        }
        mind = min (mind, dist);
    }


    /*
    */
    return  false;
}
bool detectint2 (double x, double y, double z) {
    vector<double> v(3);
    v[0] = x * x + (y + z) * (y + z);
    v[1] = y * y + (x + z) * (x + z);
    v[2] = z * z + (x + y) * (x + y);

    for (int i = 0; i < 3; i++) {
        double dist = sqrt (v[i]);
        if (floor(dist) == dist) {
            return true;
        }
    }
    return false;
}
int main () {

    const double x = 6, y = 5, z = 3;
    const vector<double> origin = {0,0,0}, exit = {x,y,z};
    vector<double> p {0,0,0};
    double mind = 99.9, dist;

    int dim = 99; // 1975
    int cnt = 0;
    //detectint (x,y,z);

    for (double x = 1; x <= dim; x++) {
        for (double y = 1; y <= dim; y++) {
            for (double z = 1; z <= dim; z++) {
                if (detectint2 (x,y,z)) {
                    cnt++;
                }
            }
        }
    }

    cout << cnt << " ";
    /*
    cout << cnt << ' ';

    /*
    */

    cout << "\nend";

}
