#include <stdlib.h>
#include <stdbool.h>

#define SQUARES (*(sqr->array  + sqr->sz++))

typedef struct Data Data;
struct Data {
     int *array;
     int sz;
};

Data* sqInRect(int length, int width) {

    int size = 20;
    Data *sqr = malloc (sizeof (Data));
        sqr->array = malloc (size * sizeof (int));
        sqr->sz = 0;

    if (length == width) return sqr;

    while (width != length) {
        if (sqr->sz == size) {
            size += 10;
            sqr->array = realloc (sqr->array, size * sizeof(int));
        }

        if (width > length)
            width -= SQUARES = length;
        else
            length -= SQUARES = width;


    }

    SQUARES = width;

    return sqr;
}
