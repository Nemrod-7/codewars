#include <stddef.h>

size_t countBits(unsigned value) {

  size_t cnt = 0;

  while (value > 0) {
    if (value % 2 == 1) cnt++;
    value = value / 2;
  }
  return cnt;
}
