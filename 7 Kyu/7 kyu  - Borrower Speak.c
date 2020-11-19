#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char *borrow(const char *source) {

  char *res = malloc (strlen(source) * sizeof (char));
  int i = 0;

  do {
    if (isalpha(*source))
      res[i++] = tolower(*source);
  }
  while (*source++ != '\0');

  res[i] = '\0';

return res;
}
