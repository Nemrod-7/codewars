#include <stdbool.h>
#include <stdlib.h>

#define BUFFSIZE 10000

int dblLinear (int n) {
    int *u = malloc (n * 2 * sizeof(int)), next = 1, x, y, z, a = 0, b = 0;
    u[0] = 1;

    for (x = 0; x <= n; ++x) {
        y = 2 * u[a] + 1;
        z = 3 * u[b] + 1;

        if (y <= z) {
            u[next++] = y;
            a++;

            if (y == z)
              b++;

        } else {
            u[next++] = z;
            b++;
        }
    }

    return u[n];
}
