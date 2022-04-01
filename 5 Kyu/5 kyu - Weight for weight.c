#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern char *strdup (const char *);

int getweight (const char *num) {
    int sum = 0;
    for (int i = 0; num[i] != '\0'; i++) {
        sum += num[i] - '0';
    }
    return sum;
}
_Bool cmp (const void *a, const void *b) {
    const char *aa = *(const char **)a;
    const char *bb = *(const char **)b;
    int left = getweight (aa), right = getweight (bb);

    if (left == right) {
        return srtrcmp (aa, bb);
    }
    return left > right;
}

char *orderWeight(const char* s) {

    char *os = malloc (128 * sizeof (char)), *p = os;
    char *str = strdup (s), *pch = strtok (str, " ");
    char *base[64];
    int size = 0;

    while (pch) {
        base[size++] = strdup (pch);
        pch = strtok (NULL, " ");
    }

    qsort (base, size, sizeof(char*) ,cmp);

    for (int i = 0; i < size; i++) {
        p += sprintf (p, "%s ", base[i]);
        free (base[i]);
    }

    p[-1] = 0;
    return os;
}
