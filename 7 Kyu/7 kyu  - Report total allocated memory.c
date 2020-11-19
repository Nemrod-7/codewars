#include <stdlib.h>

  int alloc = 0;

void *mem_alloc(size_t size) {

  alloc += size;
  return calloc (size, sizeof (size_t));
}

unsigned long long report_total_allocated(void) {
  return alloc;
}
