#include <iostream>
#include <cmath>

bool is_square (int n) {
    const int rt = sqrt (n);
    return (rt * rt == n) ? true : false;
}

int sum_of_squares (int n) {

    if (n <= 0 || is_square (n)) return 1;
    if (n < 4) return n;

    while (n % 4 == 0) n >>= 2;

    if (n % 8 == 7) return 4;

    for (int i = 1; i * i <= n; ++i)
        if (is_square (n - i * i))
            return 2;

    return 3;
}
/*
void heapPermutation (string a, int size, vector<string> *actual) {

  if (size == 1) {
    bool unique = true;

    for (int i = 0; i < actual->size(); ++i)
    if (actual->at(i).compare(a) == 0)
    unique = false;

    if (unique == true)
    actual->push_back (a);
  }
  for (int i = 0; i < size; i++) {
    heapPermutation(a, size - 1, actual);
    size % 2 == 1 ? swap (a[0], a[size-1]) : swap (a[i], a[size-1]);
  }
}
*/
