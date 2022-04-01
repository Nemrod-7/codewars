#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ABS(x) ((x) < 0 ? -(x) : (x))
extern char *strdup (const char *);

typedef struct _tuple { int weight, pos; long long num; } tuple;

int compare (const void* a, const void* b) {
    tuple arg1 = *(const tuple*)a, arg2 = *(const tuple*)b;
    return arg1.weight > arg2.weight;
}
int getweight (long long x) {
    int sum = 0;
    do { sum += x % 10; } while (x /= 10);
    return sum;
}
char *closest (const char *src) {

    char *str = strdup (src), *pch = strtok (str, " "), *os = malloc (64 * sizeof (char));
    size_t size = 0;
    tuple base[64], res[2];

    while (pch) {
        int num = atoi(pch), weight = getweight (num);
        base[size] = (tuple) {weight, size, num};
        size++;
        pch = strtok (NULL, " ");
    }
    if (size == 0) return strdup ("{{0,0,0},{0,0,0}}");

    qsort (base, size, sizeof (tuple), compare);
    int minv = 9999;

    for (size_t i = 0; i < size - 1; i++) {
        int dist = ABS (base[i].weight - base[i + 1].weight);

        if (dist < minv) {
            res[0] = base[i], res[1] = base[i + 1];
            minv = dist;
        }
    }

    sprintf (os,"{{%d, %d, %lld}, {%d, %d, %lld}}",res[0].weight,res[0].pos,res[0].num,res[1].weight,res[1].pos,res[1].num);
    return os;
}
