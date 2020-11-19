#include <stddef.h>
#include <stdlib.h>

char *decode(const unsigned char *code, size_t n, unsigned k) {

  int i, next = 0;
  char key[16], *dcryted = malloc (strlen(code) * sizeof(char));
  sprintf (key,"%i",k);

  for (i = 0; i < n; ++i) {
    if (key[next] == NULL) next = 0;
      dcryted[i] = (code[i] + 'a' - 1) - (key[next++] - '0');
  }
  dcryted[i] = NULL;
  return dcryted;
}
