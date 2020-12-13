#include <stdio.h>
#include <stdlib.h>
#include <math.h>

char* factors(int lst) {

    char *out = malloc (64 * sizeof(char)), *ptr = out;

    for (int p = 2; p < sqrt (lst); p++)
        if (lst % p == 0)
            for (int k = 1; k <= sqrt (lst); k++) {
                if (lst % p != 0) {
                    if (k - 1 > 1 ) ptr += sprintf (ptr, "(%i**%i)",p,k - 1);
                    else ptr += sprintf (ptr, "(%i)",p );
                    break;
                }
                lst /= p;
            }

    if (lst > 2) ptr += sprintf (ptr, "(%i)",lst);

    return out;
} 
