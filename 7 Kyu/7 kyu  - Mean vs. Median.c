#include <stdlib.h> // size_t

int compare (const void * a, const void * b) {
return ( *(int*)a - *(int*)b );
}


const char* meanVsMedian (int *numbers, size_t numbersSize) {

  int i = 0, mean = 0, med = 0, sum = 0;

  qsort (numbers, numbersSize, sizeof(int), compare);

  while (i < numbersSize) {
    mean += numbers[i++];
    med = numbers[i / 2] * i;
  }

  if (mean > med)
    return "mean";
  else if (mean < med)
    return "median";
  else if (mean == med)
    return "same";
}
