#include <iostream>
#include <cmath>

using namespace std;

int main () {


    double b = 6, h = 4;
    double a = 5;

    for (int a = 2; a < 1200; a++) {
        for (int i = -1; i < 2; i += 2) {
            int b = a + i;

            double bn = b / 2;
            double h = sqrt (a * a - bn * bn);

            double peri = 2 * a + b;
            double area = b / 2 * sqrt (a * a - (b * b) / 4);

            if (abs (area - round(area)) < 1e-8) {

                cout << a << " " << b  << " " << area << endl;
            }
        }
    }

}
