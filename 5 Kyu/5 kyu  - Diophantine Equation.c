#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

#define DATA (*(data + next))

typedef struct Pair Pair;
struct Pair {
    long long first;
    long long snd;
};

Pair **createpair (int size) {

    Pair **new = malloc (size * sizeof (Pair));

    for (int i = 0; i < size; ++i)
        new[i] = malloc (sizeof (Pair));

    return new;
}
Pair** solEquaStr(long long n, int* length) {

    const int64_t square = sqrt (n);
    int64_t y, x,temp, next = 0, i;
    Pair **data = createpair (square);

    for (i = 1; i <= square; i++) {
        temp = n / i;
        y = (temp - i) * 0.25;
        x = i + 2 * y;

        if (n == (x - 2 * y) * (x + 2 * y)) {
            DATA->first = x,  DATA->snd = y;
            next++;
        }
     }

    *length = next;

    return data;
}
