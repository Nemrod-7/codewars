#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <chrono>

using namespace std;


int diophantine1 (int d) {

    for (int x = 2; x < 10000; x++) {
        for (int y = 1; y < x; y++) {
            if ((x * x) - d * (y * y) == 1) {
                cout << x << "^2 - " << d << " * " << y << "^2";

                cout << " :: " <<  d * (y * y);
                return x;
            }
        }
    }

    return 0;
}
int diophantine2 (int d) {
    double sqd = -sqrt (d);
    for (int x = 2; x < 10000; x++) {
        for (int y = 1; y < x; y++) {
            if (x + sqd * y == 1) {
                cout << x << "^2 - " << d << " * " << y << "^2";

                return x;
            }
        }
    }

    return 0;
}
bool square (int x) {
    int sq = sqrt(x);
    return (sq * sq) == x;
}

int main () {

  auto start = std::chrono::high_resolution_clock::now();


  int n;
  double t ;

    for (int i = 2; i < 50; i++) {
        if (sqr[i] == false) {

            cout << i << " = > ";
            int x = diophantine1 (i);
            int eq = sqrt (x * x) - i;

            cout << " :: "  << "\n";
            if ( x > maxv ) {
                maxv = x;
                res = i;
            }

        }
    }

    */

    /*
     * x^2 - z * y^2 == 1
     * sqrt (-z * y^2)  == 1 / (x^2 )
     *
     */
     auto end = std::chrono::high_resolution_clock::now();
     std::chrono::duration<double> elapsed = end - start;
     std::cout << "\nProcess took " << elapsed.count()  << " ms" << std::endl;
  }
