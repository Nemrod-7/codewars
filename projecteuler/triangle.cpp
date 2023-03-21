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

double rnd (double x) { return round (x * 100) / 100.0; }
vector<double> tokenize (const string &src, char delim) {
    istringstream iss (src);
    string token;
    vector<double> v;

    while (getline (iss, token, delim))
        v.push_back (stoi(token));

    return v;
}

pair<double,double> barycentre (point p1, point p2, point p3) {
    double x = (p1.first + p2.first + p3.first) / 3;
    double y = (p1.second + p2.second + p3.second) / 3;
    return {x, y};
}
double area (point a, point b, point c) {
    double x1 = a.first, x2 = b.first, x3 = c.first;
    double y1 = a.second, y2 = b.second, y3 = c.second;
    return rnd (abs (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2);
}

int main () {

    // problem 102 : Triangle containment

    ifstream file ("files/p102_triangles.txt");

    const double epsilon = 1e-06;
    const point origin = {0,0};

    int cnt = 0;
    string line;

    while (getline (file, line)) {

        vector<double> vec = tokenize (line, ',');
        point a = {vec[0], vec[1]}, b = {vec[2],vec[3]}, c = {vec[4],vec[5]};

        double total = area (a,b,c);
        double aa = area (a, b, origin), bb = area (a,origin, c), cc = area (origin, b, c);
        double trigl = aa + bb + cc;

        if (abs (total - trigl) <= epsilon) {
            cnt++;
        } else {
            //cout << abs (total - trigl) << ' ';
            //cout << aa + bb + cc << " " << total;
            //cout << " \n";
        }
    }

    cout << "res :: " << cnt;

    file.close();

}
