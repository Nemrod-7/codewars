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

    for (int i = cut + 1; num[i] != '\0'; ++i) {
        for (int j = i + 1; j < num[j] != '\0'; j++) {
            if (num[i] < num[j]) {
                swap(&num[i],&num[j]);
            }
        }
    }

    printf("%d %d %s end\n", cut, up, num);
    /*
    left = &num[cut + 1], right = &num[len - 1];
    while (left < right ) {
        swap(left, right);
        printf("%d %d %s\n", cut, up, num);
        left++, right--;
    }
    */
    return (num[0] == '0') ? -1 : atoll (num);
}

int main () {

    next_smaller_number(907);

    /*
    next_smaller_number(21);
    next_smaller_number(531), 513);
    next_smaller_number(1027), -1);
    next_smaller_number(441), 414);
    next_smaller_number(123456798), 123456789);
    */


}
