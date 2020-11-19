typedef struct _array_minmax_t {
  int min,max;
} array_minmax_t;

array_minmax_t *array_minmax_create(unsigned int length, int *input_array) {
  array_minmax_t *new = malloc (sizeof (array_minmax_t));
  new->min = new->max = *input_array;
  while (length-->0) {
      int num = *((input_array) + length);

      if (num < new->min)
          new->min = num;

      if (num > new->max)
          new->max = num;
  }
  return new;
}

void array_minmax_add(array_minmax_t *array, int number) {
  if (number < array->min)
      array->min = number;
  if (number > array->max)
      array->max = number;
}

int array_minmax_get_min(array_minmax_t *array) { return array->min;}
int array_minmax_get_max(array_minmax_t *array) { return array->max;}
void array_minmax_destroy(array_minmax_t *array) { free (array);}
