#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFSIZE 64

void swap (char *a, char *b) {

    char temp = *a;
    *a = *b;
    *b = temp;
}
unsigned long long next_smaller_number(unsigned long long entry) {

    char num[BUFFSIZE], *right, *left, *end;

    sprintf (num,"%llu",entry);
    int len = strlen (num), cut = len - 1, up = len;

    while (cut-->0)
        if (num[cut] > num[cut + 1])
            break;

    if (cut == -1) return -1;

    while (up-->0)
      if (num[up] < num[cut])
          break;

    swap (&num[cut],&num[up]);

    left = &num[cut + 1], right = &num[len - 1];

    while (left < right ) {
        swap(left, right);
        left++, right--;
    }

    return (num[0] == '0') ? -1 : atoll (num);
}
