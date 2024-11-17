
#include <iostream>
#include <cmath>

using namespace std;

double sec (double x) { 
    return 1.0 / cos(x); 
}
double csc (double x) {
    return 1.0 / sin(x);
}

int main () {

    double a1 = cos(5);

    double x = 5.0;
    double dx = 3;

    cout << -dx / pow(sin(x), 2) << "\n";
    cout << -dx * pow(csc(x), 2) << "\n";
}
