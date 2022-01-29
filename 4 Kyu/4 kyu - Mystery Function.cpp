#include <iostream>
#include <iostream>

ulong mystery (ulong n) {
    return (n >> 1)^n;
}

ulong mysteryInv (ulong n) {
    ulong mask = n;

    while (mask) {
        mask >>= 1;
        n ^= mask;
    }

    return n;
}

std::string nameOfMystery () {
  return "Gray code";
}
int main () {

  //  0 1 2 3 4 5 6 7 8 9
  //  0 1 3 2 6 7 5 4
    int n = 9;
    std::cout << mysteryInv (13);
}
