#include <iostream>
#include <cmath>

using namespace std;

double degree (double radian) { return radian * 180.0 / M_PI; }

int main () {

    double r = 6.75;
    double hy = 8.1;

    double theta = degree (acos (r / hy));

    cout << 180 / 360.0 << " " << 1 / 180.0;
}
