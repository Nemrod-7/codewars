#include <ctype.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char *toInitials(const char *name) {

  int i;
  char *res = malloc (5 * sizeof(char));
  char *p = res;

  for (i = 0; i < strlen(name); ++i) {
    if (isupper (name[i])) {
      *p++ = name[i];
      *p++ = '.';
      *p++ = ' ';
    }
  }
  *--p = '\0';
return res;
}
