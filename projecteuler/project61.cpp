#include <iostream>

using namespace std;

int main () {

    // # problem 61
    // Cyclical figurate numbers

    /*

    Triangle 	  	P3,n=n(n+1)/2 	  	1, 3, 6, 10, 15, ...
    Square 	  	  P4,n=n2 	  	      1, 4, 9, 16, 25, ...
    Pentagonal 	  P5,n=n(3n−1)/2 	  	1, 5, 12, 22, 35, ...
    Hexagonal 	  P6,n=n(2n−1) 	  	  1, 6, 15, 28, 45, ...
    Heptagonal 	  P7,n=n(5n−3)/2 	  	1, 7, 18, 34, 55, ...
    Octagonal 	  P8,n=n(3n−2) 	  	  1, 8, 21, 40, 65, ...

    */
    const size_t lim = 10;
    for (size_t n = 0; n < lim; n++) {
        int p3 = n * (n + 1) / 2;     // triangle
        int p4 = n * n;               // square
        int p5 = n * (3 * n - 1) / 2; // pentagonal
        int p6 = n * (3 * n - 1);     // Hexagonal
        int p7 = n * (5 * n - 3) / 2; // heptagonal
        int p8 = n * (3 * n - 2);     // octogonal

    }

}
