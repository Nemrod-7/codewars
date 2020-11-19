struct arr {
	int value;
	int expanded;
};

int* sortByValueAndIndex(int* array, int arrayLength) {

	int i;
	struct arr *list = malloc (arrayLength * sizeof(struct arr));
  int compare (const void * a, const void * b) ;

	for (i = 0; i < arrayLength; ++i) {
		list[i].value = array[i];
		list[i].expanded = array[i] * (i + 1);
	}
	qsort (list,arrayLength,sizeof (struct arr), compare);

	i = arrayLength;
	while (i-->0)
		array[i] = list[i].value;

return array;
}

int compare (const void * a, const void * b) {

  struct arr *valueA = (struct arr *)a;
  struct arr *valueB = (struct arr *)b;

  return (valueA->expanded - valueB->expanded);
}
