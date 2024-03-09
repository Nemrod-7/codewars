
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint64_t big_modulo (const char* src, uint64_t div) {
    uint64_t res = 0;

    for (int i = 0; src[i] != '\0'; i++) {
        uint64_t mid = (res * 10 + (src[i] - 48));
        res = mid % div;
        printf("%i", (src[i] - 48));

        // printf("%llu\n", (src[i] - 48));
    }

    printf("\n");
    return res;
}

void assert_eq_custom (const char *src, uint64_t div, uint64_t res) {
    big_modulo(src, div);
}

int main () {

    // binomial expansion
// expand("(x+1)^2");      // returns "x^2+2x+1"
// expand("(p-1)^3");      // returns "p^3-3p^2+3p-1"
// expand("(2f+4)^6");     // returns "64f^6+768f^5+3840f^4+10240f^3+15360f^2+12288f+4096"
// expand("(-2a-4)^0");    // returns "1"
// expand("(-12t+43)^2");  // returns "144t^2-1032t+1849"
// expand("(r+0)^203");    // returns "r^203"
// expand("(-x-1)^2");     // returns "x^2+2x+1"


}
