#include <string.h>
#include <stdlib.h>

char* replaceNth(const char* text, int n, char oldValue, char newValue) {

  int i;
  int cnt = 1;
  char *res = malloc (strlen(text) * sizeof (text));

  if ( n <= 0) return text;

  i = 0;
  while (i < strlen(text)) {
    if (text[i] != oldValue) {
      res[i] = text[i];
    }
    else if (text[i] == oldValue && cnt < n) {
      res[i] = text[i];
      ++cnt;
    }
    else if (text[i] == oldValue && cnt == n) {
      res[i] = newValue;
      cnt = 1;
    }
    ++i;
  }
res[i] = '\0';

return res;
}
