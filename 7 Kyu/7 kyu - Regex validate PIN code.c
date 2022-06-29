#include <regex.h>
#include <stdbool.h>

bool validate_pin(const char *pin) {
    regex_t regex;
    regcomp (&regex, "(^[0-9]{4}$|^[0-9]{6}$)", REG_EXTENDED);
    return regexec (&regex, pin, 0, 0, 0) == false;
}
