#include <stdio.h>
#include <stdlib.h>


int main () {

    // Input [5, 4, 3, 2, 1] => Output [4, 3, 2, 1, 0]
    // Input [1, 2, 0] => Output [1, 1, 0]
    const int size = 5;
    const int arr[] = {5,4,3,2,1};

    for (int i = 0; i < size; i++) {
        int cnt = 0;

        for (int j = i + 1; j < size; j++) {
            if (arr[j] < arr[i]) {
                cnt++;
            }
        }
        printf("%i ", cnt);
    }



}
