#include <stdlib.h>

#define MIN(x,y) ((x) < (y)) ? (x) : (y)

typedef void* (*Function)(const void*, const void*);

void *lzip(
  const void *a, size_t a_count, size_t a_item_size,
  const void *b, size_t b_count, size_t b_item_size,
  size_t result_item_size, Function func)
{

    size_t buff = MIN (a_count,b_count);
    char **Arr = malloc(buff * sizeof(char*));
    int i;

    for (i = 0; i < buff; ++i)
      Arr[i] = func(a + i * a_item_size,b + i * b_item_size);


return Arr;
}
