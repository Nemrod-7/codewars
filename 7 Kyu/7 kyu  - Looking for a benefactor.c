#include <stdlib.h>
#include <math.h>

long long newAvg(double *arr, size_t szArray, double navg)
{

  double sum = 0, res = 0;
  long long total = szArray;

  while (szArray-- ) {
    sum += *arr++;
  }

  if ((res = (navg * (total + 1) - sum)) <0)
    return -1;

return ceil(res);
}
