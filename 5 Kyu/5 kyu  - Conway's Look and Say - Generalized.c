#include <stdio.h>
#include <stdlib.h>

#define shift(x) ((int)pow (10,(int)log10(x) + 1))
typedef unsigned long long long_num;

long_num look_say(long_num number) {

  if (number == 0) {
    return 10;
  }
  long_num concat(int x, int y);
  int actual, next, cnt = 1, len = (int) log10 (number);
  long_num index = pow (10, len), res = 0;

    do {
    actual = (number / (index)) % 10;
    next = (index > 1 ) ? (number / (index / 10)) % 10 : 'x';

    if (actual == next)
      cnt++;

    if (actual != next) {
      res = res * shift(cnt) + cnt;
      res = res * 10 + actual;

      if (cnt > 1)
        cnt = 1;
    }

  }
  while (index /= 10);

  return res;
}
