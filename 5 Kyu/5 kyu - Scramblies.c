#include <stdbool.h>

#define DIGIT(x) ((x) - 'a')
#define ALPHA 26

bool scramble(const char *str1, const char *str2) {

    int stack[ALPHA] = {0}, index = ALPHA;
    char c;

    while (c = *str1++) stack[DIGIT(c)]++;

    while (c = *str2++)
        if (--stack[DIGIT(c)] < 0)
            return false;

    return true;
}
