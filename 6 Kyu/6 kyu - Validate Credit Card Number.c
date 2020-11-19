#include <stdio.h>
#include <stdbool.h>

#define IS_ODD(x) (x % 2 == 1)

bool validate(long number) {

    int dig, cnt = 0, sum = 0;

    while (number) {
        dig = number % 10;
        number /= 10;

        if (IS_ODD(cnt++)) dig *= 2;
        if (dig > 9) dig -= 9;

        sum += dig;
    }
    return sum % 10 == false;
}
