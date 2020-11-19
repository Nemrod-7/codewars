#include <stddef.h> // NULL
#include <ctype.h>

char *to_alternating_case(char *string) {

char *link = string;

	do {
		*link = islower(*link) ? toupper(*link) : tolower(*link);
		++link;
	}
	while (*link);

  return string;
}
