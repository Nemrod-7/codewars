
#include <iostream>
#include <cstdlib>
#include <cstring>

int solve(const char *src) {
    int res = 0 ;
    int xs = 1, sign = 1, left = 1;
    char *eq = strdup(src), *tok = strtok(eq, " ");

    while (tok) {

        if (strcmp(tok, "+") == 0) {
            sign = left;
        } else if (strcmp(tok, "-") == 0) {
            sign = -left;
        } else if (strcmp(tok, "=") == 0) {
            sign = -1, left = -1;
        } else if (strcmp(tok, "x") == 0) {
            xs = -sign;
        } else {
            res += atoi(tok) * sign;
        }
        // printf("[%s]", tok);

        tok = strtok(NULL, " ");
    }

    return res * xs;
}

int main () {

    int res = solve ("x + 1 = 9 - 2");
    printf ("%i ", res);

    return 0;
}
