#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ARR_LEN(arr) (sizeof(arr) / sizeof(*arr))

void csv_columns (const char *csv, size_t size, size_t indices[size], char *select) {

    char *ptr = select;
    char line[64], *col = NULL;
    size_t len = strlen (csv), ncol = 0;
    _Bool idx[64] = {};

    for (int i = 0; csv[i] != '\n'; i++) {
        if (csv[i] == ',') ncol++;
    }

    for (int i = 0; i < size; i++) {
        idx[indices[i]] = 1;
    }

    for (size_t i = 0; i < len; ) {
        sscanf (csv + i, "%s\n", line);
        i += strlen (line) + 1;

        col = strtok (line, ",");
        size_t j = 0;
        _Bool found = 0;

        while (col) {
            if (idx[j] == 1) {
                ptr += sprintf(ptr, "%s,", col);
                found = 1;
            }

            col = strtok (NULL, ",");
            j++;
        }

        if (found) {
            ptr--;
            ptr += sprintf(ptr, "\n");
        }
    }
    
    *(ptr - 1) = '\0';
}

int main () {

    const char str[] =
    "f,w88g,o,VpRu,h6Ya\n"
"Kd7Mg,niI,7LKnI,vzT,eza5u\n"
"n0,Oj72q,iDH,VwZ,HM9lI9\n"
"3n,dt,r,8e5rl,i\n"
"qv,IRTs,lE,WXb,aOzDI\n"
"5,Gj48VC,k3mHLD,DWWSy,0\n"
"oayn,HvuGB,LJ8Y,tb,BwAwAg\n"
"dfW,FLEE,3,3xmF,7Zjf\n"
"dGgoD,lOXF1D,fT6th,wD,tz";

    size_t indices[] = {5, 2, 2, 3, 8, 9, 9, 1, 6, 7, 3};
    const size_t size = ARR_LEN (indices);

    int ncol = 0;
    for (int i = 0; str[i] != '\n'; i++) {
        if (str[i] == ',') ncol++;
    }

    _Bool idx[64] = {};

    for (int i = 0; i < size; i++) {
        idx[indices[i]] = 1;
    }

    for (int i = 0; i < ncol; i++) {
        if (idx[i])
            printf ("%d ", i);
    }

}
