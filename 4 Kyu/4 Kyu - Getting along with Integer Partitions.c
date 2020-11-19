#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct stat {
    int range;
    double mean;
    double median;
};
int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}
struct stat compute (int *arr, int size) {

    struct stat actual = {0};
    int next = 0, data[size];

    qsort (arr,size,sizeof(int),cmpfunc);

    for (int i = 0; i < size; ++i)
        if (arr[i] != arr[i+1])
            actual.mean += data[next++] = arr[i];

    actual.median = (next % 2 == 0) ? ((data[next/2] + data[next/2 - 1]) / 2.0) : data[next/2];

    actual.range = data[next - 1] - 1;
    actual.mean /= (float)next;

    return actual;
}
int mult (int *arr, int size) {

    int prod = 1;
    for (int i = 0; i < size; ++i)
        prod *= arr[i];

    return prod;
}
char* part(int n) {

    int mem = 64, size = 0, *prods = malloc (mem * sizeof(int));
    char *output = malloc (64 * sizeof (char));
    int next = 0, stack, digit, index = 0, memstk = 0, part[n];
    struct stat actual;

    memset (part, 0, sizeof(part));
    part[index] = n + 1;

    while (part[0] > 1) {

        digit = part[index] - 1;
        stack = memstk, next = index;

        while (stack < n)
            if (n - stack >= digit) {
                if (digit > 1) {
                    index = next, memstk = stack;
                }
                stack += part[next++] = digit;

            }
            else
                digit--;

        prods[size++] = mult (part,next);

        if (size == mem) {
            mem += mem;
            prods = realloc (prods, mem * sizeof (int));
        }

        if (part[index] == 1) index--, memstk -= part[index];
        //if (part[0] == 1) break;
    }

    actual = compute (prods,size);
    sprintf (output,"Range: %i Average: %.2f Median: %.02f",actual.range,actual.mean,actual.median);

    return output;
}

int main () {

    part (4);
}
