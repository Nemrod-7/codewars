#include <stdio.h>
#include <stdlib.h>

char *histogram (int data[6]) {

    int idx = 6;
    char *out = malloc (512 * sizeof(char)), *ptr = out;

    while (idx-->0) {
        int dig = data[idx];
        ptr += sprintf (ptr, "%i|", idx + 1);

        if (dig != 0) {
            for (int i = 0; i < dig; i++)
                ptr += sprintf (ptr, "#");

            ptr += sprintf (ptr, " %i", dig);
        }
        ptr += sprintf (ptr, "\n");
    }

    return out;
}
