#include <stdio.h>
#include <ctype.h>

int scorer (const char *src) {

    int score = 0;

    for (int i = 0; src[i] != '\0'; i++) {
        if (isdigit (src[i])) {
            score += src[i] - '0';
        } else {
            if (src[i] == 'A')
                score += 1;
            else
                score += 10;
        }
    }

    return score;
}
int main () {

    printf ("%d", scorer ("A"));
}
