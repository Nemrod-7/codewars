#include <stdio.h>



int divisors(int n) {

    int index = n + 1, count = 0;

    while (index-->1)
        if (n % index == 0)
            count++;
    return count;
}

void main () {


    printf ("%i",divisors (500000));
}
