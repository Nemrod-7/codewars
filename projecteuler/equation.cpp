#include <iostream>
#include <vector>
#include <cmath>

using namespace std;


int diophantine (int d) {

    for (int x = 2; x < 10000; x++) {
        for (int y = 1; y < x; y++) {
            if ((x * x) - d * (y * y) == 1) {
              cout << x << "^2 - " << d << " * " << y << "^2\n";
                return x;
            }
        }
    }

    return 0;
}



int main () {



    cout << endl;
}
