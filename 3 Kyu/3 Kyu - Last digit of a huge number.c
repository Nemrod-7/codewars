#include <stdlib.h>
#include <math.h>


int last_digit(const unsigned long int *arr, size_t arr_size);


int main () {
    const unsigned long int arr[] = {302444,161528,332700,23787,242589,799426};
    printf ("%i\n",last_digit(arr,6));

}


int last_digit(const unsigned long int *arr, size_t arr_size) {

    int index = arr_size;
    unsigned long int num, div ,exp;

    if (arr_size == 0) return 1;

    num = 1;
    while (index-->0) {
        exp = (arr[index] % 100 > 1) ? arr[index] % 100 : arr[index] % 1000;
        div = num < 4 ? num : num % 4 + 4;
        num = pow (exp, div);
    }

    return num % 10;
}
