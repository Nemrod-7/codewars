#include <iostream>


int main () {

    int X[81], Y[81], Z[81];

    for (int i = 0; i < 81; i++) {
        X[i] = i % 9, Y[i] = i / 9, Z[i] = Y[i] / 3 * 3 + X[i] / 3;

    }


    int ia[5], ib[5];

    ia = ib;
}
