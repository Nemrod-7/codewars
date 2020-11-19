#include <stddef.h>

struct IntegerPair {
  int key;
  int value;
};

int gcd(int a, int b);

int gcd(int a, int b) {

    if (a == 0 || b == 0)
      return 1;
    if (a == b)
      return a;
    if (a > b)
      return gcd(a - b, b);
    else
      return gcd(a, b - a);
}

int sum_differences_between_products_and_LCMs(struct IntegerPair *pairs, size_t count){

  int i = count - 1, res = 0;
  int prod[count], least[count];

  do {
    prod[i] = pairs[i].key * pairs[i].value;
    least[i] = prod[i] / gcd (pairs[i].key, pairs[i].value);
    res += (prod[i] - least[i]);
  }
  while (i-->0);

return res;
}
