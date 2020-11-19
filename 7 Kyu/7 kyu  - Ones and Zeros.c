#include <stddef.h>

unsigned binary_array_to_numbers(const unsigned *bits, size_t count){

  int i, num = 0;

  for (i = 0; i < count; ++i) {
    num = num * 2 + bits[i];
  }

return num;
}
