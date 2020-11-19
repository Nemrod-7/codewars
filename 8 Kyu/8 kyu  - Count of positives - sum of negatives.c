#include <stddef.h>

void count_positives_sum_negatives (
  int *values, size_t count, int *positivesCount, int *negativesSum) {

   const int *end = values + count;

      for (; values < end; ++values) {
          if (*values <= 0) {
              *negativesSum += *values;
          } else {
              *positivesCount += 1;
          }
      }
}
