char* isSortedAndHow(int *array, int arrayLength) {

  int i = 0;
  int start = array[0], next = array[1];
  int ascending = next > start;
  int descending = next < start;

  for (i = 0; i < arrayLength ; ++i) {
    if (array[i] < start && ascending) return "no";
    if (array[i] > start && descending) return "no";
  }

return (array[i] > start && ascending) ? "yes, ascending" : "yes, descending";
}
