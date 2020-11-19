#include <stddef.h>

unsigned int countBits(unsigned long long n){

    size_t cnt = 0;

    while (n > 0) {
      if (n % 2 == 1) cnt++;
      n /= 2;
    }
    return cnt;
}
