#include <stdlib.h>

#define BUFFSIZE 2048

char* collatz(int n) {

    char *output = malloc (BUFFSIZE * sizeof(char)), *out = output;

    while (n != 1) {
        out += sprintf (out,"%i->",n);
        n = (n % 2 == 0) ? n * 0.5 : 3 * n + 1;
    }
    out += sprintf (out,"1");

    return output;
}
