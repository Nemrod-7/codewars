#include <string.h>
#include <stdlib.h>

#define BUFFSIZE 64

void swap (char *a, char *b) {

    char temp = *a;
    *a = *b;
    *b = temp;
}
unsigned long long next_bigger_number(unsigned long long entry) {

    char num[BUFFSIZE];

    sprintf (num,"%llu",entry);
    int up, pos, len = strlen (num);

    if (len <= 1) return -1;

    up = pos = len;

    while (pos-->0)
        if (num[pos] < num[pos + 1])
            break;

    if (pos == -1) return -1;

    while (up-->0)
        if (num[up] > num[pos])
            break;

    swap (&num[pos],&num[up]);

    for (int i = pos + 1; num[i] != '\0'; ++i)
        for (int j = i + 1; j < num[j] != '\0'; j++)
            if (num[i] > num[j])
                swap(&num[i],&num[j]);

    return strtoull(num,NULL, 10);
}
