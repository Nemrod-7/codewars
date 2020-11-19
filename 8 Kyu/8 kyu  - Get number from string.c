#include <string.h>

char *repeat_str(size_t count, const char *src) {

char *res = malloc(strlen(src) * count);

strcpy(res,src);
while (--count > 0) strcat (res,src);

return res;
}
