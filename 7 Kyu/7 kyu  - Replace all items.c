int *replaceAll (int *array, int n, int old, int new) {

  int i;
  for (i = 0; i < n; ++i)
    array[i] = new;

return array;
}
