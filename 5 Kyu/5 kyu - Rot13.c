#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MODE(x) (islower(x) ? 122 : 90 )
#define ROT13(x) ((((x) + 13) > MODE(x)) ? (-13) : (13))

char *rot13(const char *src) {

    int i = 0;
    char c, *res = malloc (strlen (src) * sizeof (char));

    while ((c = *src++)) {
        if (isalpha(c))
            c += ROT13(c);

        res[i++] = c;
    }
    res[i] = '\0';

    return res;
}
