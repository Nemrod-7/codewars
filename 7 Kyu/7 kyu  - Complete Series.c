int* completeSeries(int* arr, int arrLength) {

  int i;
  int end = 0, start = 0;

  for (i = 0; i < arrLength; ++i) {
    if (arr[i] > end) end = arr[i];
  }

  int *item = malloc ((end + 1) * sizeof (int));
  for (i = 0; i <= end; ++i) {
    item[i] = i;
  }

return item;
}
