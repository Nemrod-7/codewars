#include <string.h>
#include <stdbool.h>

#define LASTCHAR(x) (*(x + strlen(x) - 1))

int correct_tail(const char *body, const char *tail) {
  return (LASTCHAR(body) == LASTCHAR(tail)) ? true : false;
}
