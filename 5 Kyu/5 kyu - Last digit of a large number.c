#include <string.h>
#include <stdlib.h>

int getnum (const char *x) {
    int size = strlen (x);
    return (size < 3) ? atoi (x) : atoi (x + (size - 2));
}
int last_digit (const char *a, const char *b) {

    if (strcmp (b, "0") == 0) return 1;
    if (strcmp (a, "1") == 0) return 1;

    int base[10][4] = {{0,0,0,0},{1,1,1,1},{6,2,4,8},{1,3,9,7},{6,4,6,4},{5,5,5,5},{6,6,6,6},{1,7,9,3},{6,8,4,2},{1,9,1,9}};
    return base[getnum(a) % 10][getnum(b) % 4];
}
