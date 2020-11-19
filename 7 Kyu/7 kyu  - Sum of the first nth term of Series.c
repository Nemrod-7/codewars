#include <stddef.h>

char *series_sum(const size_t n) {

  int i;
  float sum = 0;
  char *buff = (char *)malloc(3);

  for (i = 1; i < n * 3; i += 3)
    sum += (float)1 / i;



  sprintf (buff,"%.2f",sum);

return buff;
}
