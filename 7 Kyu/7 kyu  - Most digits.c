#include <stddef.h>
#define RANGE(x) (((x) > 9)||((x) < -9))

int find_longest(int *numbers, size_t numbers_size) {

  size_t CntNum (int source);
  int i;
  size_t now = 0, max = 0, res = 0;

  for (i = 0; i < numbers_size; ++i) {
    now = CntNum (numbers[i]);
	if (now > max) {
		max = now;
		res = numbers[i];
	}
  }

return res;
}

size_t CntNum (int source) {

  size_t cnt = 1;

  while (RANGE(source)) {
    cnt++;
    source /= 10;
  }

return cnt;
}
