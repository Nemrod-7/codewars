#include <iostream>

bool cantor (double x, double y, double num, unsigned cycle, unsigned lim) {

    if (num < x || num > y) return false;
    if (cycle >= lim) return true;
    const double md = y / 3.0;
    const bool left = cantor (x, md, num, cycle + 1, lim);
    const bool right = cantor (y - md, y, num, cycle + 1, lim);
    // printf ("%f ", md);
    printf ("%f %f %f\n", x, num, y);

    if (left || right) return true;
    return false;
}
bool is_in_cantor (unsigned num, unsigned den, unsigned n) {


  if (num > den) return false;
  const double frac = num / (double) den;

  return cantor (0.0, 1.0, frac, 0, n);
}

int main () {
  /*
  num = 1
  den = 613999
  n   = 40
  */
    bool res = is_in_cantor (1,37,6); // false

    switch (res) {
        case 0 : printf ("false"); break;
        case 1 : printf ("true"); break;
    }
}
