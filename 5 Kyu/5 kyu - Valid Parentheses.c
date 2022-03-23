#include <stdbool.h>

bool validParentheses(const char* strin) {

    int i, left = 0, right = 0;

    for (i = 0; strin[i] != '\0'; ++i) {
        if (right > left) return false;
        if (strin[i] == '(') left++;
        if (strin[i] == ')') right++;
    }

return ((left == right) ? true : false);
}
