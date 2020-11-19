#include <stdbool.h>
#define IsLowerCase(x) (((x) >= 'a') && ((x) <= 'z'))

bool is_uppercase(const char *source) {

	const char *link = source;
	int locnt = 0;

	while (*link != '\0') {
		IsLowerCase(*link) ? ++locnt : 0;
		++link;
	}

return (locnt != 0) ? false : true;
}
