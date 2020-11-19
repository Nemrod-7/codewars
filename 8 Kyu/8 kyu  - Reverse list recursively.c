#include <stdlib.h>

int* revR (int* array, int n) {

    int i = 0,j = n - 1,temp;

    if (i < j) {
        temp = array[i];
        array[i] = array[j];
        array[j] = temp;
        revR (array + 1,n - 2);
    }
    return array;
}
