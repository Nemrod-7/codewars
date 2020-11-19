#include <stdbool.h>
#include <string.h>

bool solution(const char *string, const char *ending) {

  int i = strlen (string), j = strlen (ending);
  const int tok = i - strlen (ending);
  char buffer[j];

  if (i < j) {
    return false;
  }

  while (i-->tok) {
    buffer[--j] = string[i];
  }
  buffer[strlen (ending)] = '\0';

  return (strcmp(ending,buffer) == 0 ? true : false);
}
