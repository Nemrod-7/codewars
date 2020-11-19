#include <stdlib.h>

typedef unsigned long long ull;

ull *mk_res (ull first, ull second, ull flag) {
    ull *out = malloc (3 * sizeof (ull));
    out[0] = first, out[1] = second, out[2] = flag;
    return out;
}

unsigned long long* productFib(ull prod) {

    ull first = 0, second = 1, temp;

    while (first * second < prod) {
        temp = first;
        first = second;
        second = temp + second;

        if (first * second == prod) return mk_res (first, second, 1);
    }


    return mk_res (first, second, 0);
}
