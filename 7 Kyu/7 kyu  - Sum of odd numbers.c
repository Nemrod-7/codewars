#include <inttypes.h>

enum kBool{FALSE,TRUE};

uint64_t rowSumOddNumbers(uint32_t n) {

  uint64_t sum = 0;
  uint64_t i= 0, j = n;
  uint64_t first = 0, last = 0;

  while (n--) i += n;

      first = i * 2 + 1;
      last = (i + j) * 2 - 1;
      printf ("\n");

   do {
      if (last % 2 == TRUE) {
        sum += last;
      }
    }
    while (last-- >first);

return sum;
}
