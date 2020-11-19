#include <ctype.h>

char *alternateCase (char *string) {

char *link = string;

while (*link) {
  *link = islower(*link) ? toupper(*link) : tolower(*link);
  ++link;
}
return string;
}
