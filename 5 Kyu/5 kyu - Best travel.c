#include <stddef.h>
#include <stdbool.h>

void swap (bool *a, bool *b) {
    bool tmp = *a;
    *a = *b;
    *b = tmp;
}
bool next_permutation (bool array[], size_t length) {

    if (length == 0) return false;
    size_t i = length - 1, j = i;

    while (i > 0 && array[i - 1] >= array[i])
        i--;

    if (i == 0) return false;

    while (array[j] <= array[i - 1])
        j--;

    swap (&array[j], &array[i - 1]);

    j = length - 1;
    while (i < j) {
        swap (&array[i], &array[j]);
        i++, j--;
    }
    return true;
}

int chooseBestSum (int t, int k, int ls[], int size) {
    
    if (k > size) return -1; 
    int maxv = -1;
    bool mask[size];
  
    for (int i = 0; i < size; i++) 
        mask[i] = i < (size - k) ? 0 : 1;
  
    do {
        int sum = 0;
        for (int i = 0; i < size; i++) {
            if (mask[i]) sum += ls[i];
        }
        if (sum <= t && sum >= maxv) maxv = sum; 
        
    } while (next_permutation (mask, size));
    
    return maxv;
}
