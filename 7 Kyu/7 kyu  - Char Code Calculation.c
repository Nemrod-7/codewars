#include <stdio.h>

#define FIRSTDIG(x) ((x)/10%10)
#define SECONDIG(x) ((x)%10)

int calc(const char *source) {

int res = 0;

  do {
    if (FIRSTDIG(*source) == 7 || SECONDIG(*source) == 7) res += 6;
  }
  while (*source++);

return res;
}
int main () {
    int num = calc ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");

    printf ("%i", num);
}
