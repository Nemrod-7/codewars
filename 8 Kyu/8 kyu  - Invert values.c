#include <stddef.h>

void invert(int *values, size_t values_size) {
  const int *end = values + values_size;

	for (;values < end; ++values)
		*values = - *values ;
}
