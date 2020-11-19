#include <stdlib.h>
#include <string.h>
#include <math.h>

unsigned char *encode(const char *s, unsigned k) {

  char key[10], *code = malloc (strlen(s) * sizeof(char));
  int i, next = 0;

  sprintf (key,"%i",k);

  for (i = 0; s[i] != NULL; ++i) {
    if (key[next] == NULL) next = 0;

    code[i] = (s[i] - 'a' + 1) + (key[next++] - '0');
  }
  return code;
}
