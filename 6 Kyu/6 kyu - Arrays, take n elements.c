#include <stddef.h> /* NULL */

int *ary_take
(const int *ary, size_t ary_size, size_t n, size_t *res_size) {

    size_t res = 0;
    int i;

    if (n == 0) {
        res_size = 0;
        return NULL;
    }
    else if (n < ary_size) res = n;
    else if (n >= ary_size) res = ary_size;

    int *arr = malloc (res * sizeof(size_t));

    for (i = 0; i < res; ++i) {
        arr[i] = ary[i];
        printf ("arr: %d",arr[i]);
    }
        *res_size = res;
     //printf ("arr: %d\n",*arr);
return arr;
}
