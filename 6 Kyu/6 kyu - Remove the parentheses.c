#include <string.h>

void remove_parentheses(const char *str_in, char *str_out) {

    int pile = 0;

    do {
        if (*str_in == '(') pile++;
        if (pile == 0) *str_out++ = *str_in;
        if (*str_in == ')') pile--;
    } while (*str_in++);

    *str_out = '\0';
}
