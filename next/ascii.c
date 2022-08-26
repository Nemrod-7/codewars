#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

wchar_t *x (unsigned n, wchar_t X_string[n * (n + 1) + 1]) {
    int index = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            X_string[index++] = (j == i || j == n - i - 1) ? L'■' : L' ';
        }
        X_string[index++] = L'\n';
    }
    X_string[index-1] = L'\0';

  	return X_string;
}
char *dot (unsigned width, unsigned height) {
    const char fst[] = "+---", scd[] = "| o ";

    char *os = malloc (18 * height * width * sizeof (char)), *ptr = os;

    for (unsigned i = 0; i < height; i++) {
        for (unsigned j = 0; j < width; j++) {
            ptr += sprintf (ptr,"%s", fst);
        }
        ptr += sprintf (ptr,"+\n");
        for (unsigned j = 0; j < width; j++) {
            ptr += sprintf (ptr,"%s", scd);
        }
        ptr += sprintf(ptr,"|\n");
    }

    for (unsigned j = 0; j < width; j++) {
        ptr += sprintf (ptr,"%s", fst);
    }
    ptr += sprintf (ptr,"+\0");

    return os;
}

int main () {



    printf ("%s", dot (15,15));
}
