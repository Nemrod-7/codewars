#include <stdlib.h>
#include <ctype.h>
#include <string.h>

enum kBool {FALSE,TRUE};

char *solve(const char *str) {

    int comp(const char *src);

    _Bool flag = comp (str);
    char *res = malloc (strlen (str) + 1 * sizeof(char));

    int i = 0;
    do {
        res[i] =((flag == TRUE) ? tolower(*str) : toupper(*str));
        ++i;
    }
    while (*str++);
    res[i] = '\0';

    return res;
}

int comp(const char *src) {

    int lo = 0, up = 0;
    _Bool flag = FALSE;

    do {
        if (islower(*src)) lo++;
        if (isupper(*src)) up++;
    }
    while (*src++);

    flag = (lo >= up) ? TRUE : FALSE;

    return flag;
}
