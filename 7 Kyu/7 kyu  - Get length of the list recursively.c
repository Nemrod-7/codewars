int lenR (int *array) {

if (array == '\0')
  return 0;
else if (*array != '\0')
  return (1 + lenR (++array));
}
