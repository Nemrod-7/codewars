#include <stddef.h>

int get_average(const int *marks, size_t count) {

  const int * const end = marks + count;
  int sum = 0;

  for (;marks < end; ++marks)
    sum +=*marks;

return (sum/count);
}
