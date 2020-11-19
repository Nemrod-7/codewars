#include <stdbool.h>
#include <stddef.h>

#define VALUE (size == INT) ? *((int*)base + i) : *((long*)base + i)

enum {INT = 4,LONG = 8};

void reshape (void *base, size_t size, unsigned short first, unsigned short last) {

    unsigned short i = last, pos = first + 1;
    long val = VALUE;

    i++;
    if (size == INT) {
        while (i-->pos)
            *((int*)base + i) = *((int*)base + (i - 1));

        *((int*)base + i) = val;
    }

    if (size == LONG) {
        while (i-->pos)
            *((long*)base + i) = *((long*)base + (i - 1));

       *((long*)base + i) = val;
    }
}
size_t partition_on(void *base, size_t nmemb, size_t size, bool (*predicate)(const void *)) {

    unsigned short pos = 0;
    const int length = nmemb;

    for (int i = 0; i < length; ++i) {
        long val = VALUE;
        if (predicate (&val) == false) {
            reshape (base,size,pos,i);
            pos++;
        }
    }

    return pos;
}
