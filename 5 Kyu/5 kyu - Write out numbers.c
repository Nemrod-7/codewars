#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char base[2][20][10] = {
    {"zero","one","two","three","four","five","six","seven","eight","nine",
     "ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen","seventeen","eighteen","nineteen"},
    {".",".","twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"}};

const char mill[5][10] = {"thousand","","hundred","million"};

char *number_to_words (int num, char *str) {

    if (num < 20) {
        sprintf (str, "%s", base[0][num]);
        return str;
    }

    int buff[10], size = 0;
    int dix = num % 100;

    while (num) {
      buff[size++] = num % 10;
      num /= 10;
    }
    char *ptr = str;
    int end = dix < 20 ? 2 : 0;

    while (size-->end) {
        int dig = buff[size];

        if (dig != 0) {
            if ((size % 3) == 1) {
                ptr += sprintf (ptr, "%s", base[1][dig]);
                //if (buff[size - 1] != 0) *ptr++ = '-';
            } else {
                ptr += sprintf (ptr, "-%s ", base[0][dig]);
            }
        }
        // printf ("%c ", *(ptr - 1));
        //if (size && (size % 3 != 1) && dig) ptr += sprintf (ptr, "%s ", mill[size % 3]);

        if (dig && size % 3 == 2) ptr += sprintf (ptr, "hundred ");
        if (size == 3) ptr += sprintf (ptr, "thousand ");
        // printf ("::%i::", size % 3);
        // if (*(ptr - 1) != '-') *ptr++ = ' ';
    }
    if (dix < 20 && dix > 0)
        ptr += sprintf (ptr, "%s", base[0][dix]);

    if (*(ptr - 1) == '-' || *(ptr - 1) == ' ') {
        *(ptr - 1) = '\0';
    }

    return str;
}
char *number_to_words2 (int src, char *str) {

    if (src < 20) {
        sprintf (str, "%s", base[0][src]);
        return str;
    }

    int buffer[5], index = 0;
    char *ptr = str;

    do {
        buffer[index++] = src % 1000;
    } while (src /= 1000);

    while (index-->0) {
        int num = buffer[index];
        int cent = (num / 100) % 1000, dix = num % 100;

        if (cent) ptr += sprintf (ptr, "%s hundred ", base[0][cent]);
        if (dix < 20) {
            if (dix > 0)
                ptr += sprintf (ptr, "%s ", base[0][dix]);
        } else {
            ptr += sprintf (ptr, "%s", base[1][(dix / 10) % 10]);

            if (dix % 10 != 0) {
                ptr += sprintf (ptr, "-%s ", base[0][dix % 10]);
            } else
                *ptr++ = ' ';
        }

        if (index == 1) ptr += sprintf (ptr, "thousand ");
    }

    if (*(ptr - 1) == '-' || *(ptr - 1) == ' ') {
        *(ptr - 1) = '\0';
    }

    return str;
}
int main () {

    int src = 19327;

    char *number = malloc (128 * sizeof(char));
    //printf ("%i", num % 100);
    //"nineteen thousand three hundred twenty-seven"
    number_to_words2 (40641, number);
    printf ("[%s]", number);

}
