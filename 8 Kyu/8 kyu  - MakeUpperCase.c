#include <ctype.h>

char *makeUpperCase (char *string) {

char *link = string;

do {
  *link = toupper(*link);
  ++link;
  }
while (*link);

return string;
}
