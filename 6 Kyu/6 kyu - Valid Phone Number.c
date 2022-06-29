#include <stdio.h>
#include <stdbool.h>
#include <regex.h>

bool valid_phone_number(const char* num) {
    printf("%s\n", num);
    regex_t re;
    regcomp (&re, "^\\([0-9]{3}\\) [0-9]{3}-[0-9]{4}$", REG_EXTENDED);
    return regexec (&re, num, 0, 0, 0) == false;
}
